import datetime

import c_process
import textual.widgets as widgets
from textual.app import App, ComposeResult
from textual.containers import HorizontalGroup, Vertical, VerticalScroll
from textual.screen import ModalScreen


class TaskInputScreen(ModalScreen):
    BINDINGS = [("escape", "cancel", "Cancel")]

    def compose(self) -> ComposeResult:
        yield Vertical(
            widgets.Input(
                placeholder="Enter task name",
                type="text",
                max_length=50,
                id="name_input",
            ),
            widgets.Input(
                placeholder="Month/Day/Year Hour (e.g., 10/24/2026 14)",
                id="date_input",
            ),
            widgets.Input(
                placeholder="Tags: 0. Uncategorized, 1. Work, 2. Home, 3. Personal, 4. School",
                restrict=r"[0-4]?",
                max_length=1,
                id="tag_input",
            ),
        )

    def action_cancel(self) -> None:
        self.dismiss(None)

    def on_input_submitted(self, event: widgets.Input.Submitted) -> None:
        name_input: widgets.Input = self.query_one("#name_input", widgets.Input)
        date_input: widgets.Input = self.query_one("#date_input", widgets.Input)
        tag_input: widgets.Input = self.query_one("#tag_input", widgets.Input)

        try:
            raw_date = date_input.value.strip()
            parse_date = datetime.datetime.strptime(raw_date, "%m/%d/%Y %H")
            timestamp = int(parse_date.timestamp())

        except ValueError:
            timestamp = 0

        new_task = [name_input.value, timestamp, int(tag_input.value)]

        self.dismiss(new_task)


class Task(HorizontalGroup):
    def __init__(self, c_task):
        super().__init__()
        self.task_name = c_task.name.decode("utf-8", errors="ignore")

        try:
            self.deadline = datetime.datetime.fromtimestamp(c_task.deadline).strftime(
                "%m/%d/%Y %H:00"
            )

        except Exception:
            self.deadline = "No deadline"

        self.tag = c_task.tag

    def compose(self) -> ComposeResult:
        yield widgets.Label(f"{self.tag}", id="tag_lbl", classes="task_lbl")
        yield widgets.Label(self.task_name, id="name_lbl", classes="task_lbl")
        yield widgets.Label(self.deadline, id="date_lbl", classes="task_lbl")


class SystemC(App):
    CSS_PATH = "TUI.tcss"
    BINDINGS = [
        ("d", "toggle_dark", "Toggle dark mode"),
        ("n", "add_task", "New Task"),
        ("u", "undo_task", "UNDO"),
        ("s", "tag_search", "Search by Tag"),
        ("q", "exit", "Exit"),
    ]

    def compose(self) -> ComposeResult:
        yield widgets.Header()
        yield widgets.Footer()
        yield VerticalScroll(id="vert")

    def on_mount(
        self,
    ) -> None:  # on_mount is for stuff to happen immediately when the program starts
        try:
            c_tasks = c_process.load_all_tasks_from_c()
            vertical_scroll = self.query_one(
                "#vert", VerticalScroll
            )  # Find element with id="vert"

            if c_tasks:
                for task_item in c_tasks:
                    vertical_scroll.mount(Task(task_item))

            else:
                self.notify("No tasks found or tasks.txt is missing.\n")

        except Exception as e:
            self.notify(f"Failed to load backend tasks: {e}", severity="error")

    def action_toggle_dark(self) -> None:
        self.theme = (
            "textual-dark" if self.theme == "textual-light" else "textual-light"
        )

    def action_add_task(self) -> None:
        def check_input(task_data: list | None):
            if not task_data:  # if user pressed Escape
                return

            else:
                task_name, task_timestamp, task_tag = task_data

                c_task = c_process.Task(
                    name=task_name.encode("utf-8"),
                    deadline=task_timestamp,
                    tag=task_tag,
                )

                c_process.backend.save_task(c_task)

                vertical_scroll = self.query_one("#vert", VerticalScroll)
                vertical_scroll.mount(Task(c_task))

                self.notify(f'Added "{task_name}" successfully!\n')

        self.push_screen(TaskInputScreen(), check_input)

    # def action_undo_task(self) -> None: ...

    # def action_search_task(self) -> None: ...

    def action_exit(self) -> None:
        self.exit()


if __name__ == "__main__":
    app = SystemC()
    app.run()
