import datetime

import c_process as proc
import textual.widgets as widgets
from textual.app import App, ComposeResult
from textual.containers import HorizontalGroup, Vertical, VerticalScroll
from textual.screen import ModalScreen


class TaskInputScreen(ModalScreen):
    BINDINGS = [("escape", "cancel", "Cancel")]

    def __init__(self, add_or_search: int):
        super().__init__()

        self.add_or_search = add_or_search

    def compose(self) -> ComposeResult:
        match self.add_or_search:
            case 1:
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

            case 2:
                yield Vertical(
                    widgets.Input(
                        placeholder="Enter tag you want to see",
                        type="integer",
                        max_length=50,
                        id="search_tag_input",
                    )
                )

    def action_cancel(self) -> None:
        self.dismiss(None)

    def on_input_submitted(self, event: widgets.Input.Submitted) -> None:
        match self.add_or_search:
            case 1:
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

            case 2:
                search_tag_input: widgets.Input = self.query_one(
                    "#search_tag_input", widgets.Input
                )
                self.dismiss(search_tag_input.value)


class Task(HorizontalGroup):
    def __init__(self, c_task):
        super().__init__()
        self.task_name = c_task.name.decode("utf-8", errors="ignore").strip()

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
        ("n", "add_task", "New Task"),
        ("u", "undo_task", "UNDO"),
        ("s", "tag_search", "Search by Tag"),
        ("ctrl+r", "reveal", "Reveal"),
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
            c_tasks = proc.load_all_tasks_from_c()
            vertical_scroll = self.query_one(
                "#vert", VerticalScroll
            )  # Find element with id="vert"

            if c_tasks:
                for task_item in c_tasks:
                    vertical_scroll.mount(Task(task_item))

            else:
                self.notify(
                    "No tasks found or tasks.txt is missing.\n", severity="error"
                )

        except Exception as e:
            self.notify(f"Failed to load backend tasks: {e}", severity="error")

    def action_add_task(self) -> None:
        def check_input(task_data: list | None):
            if not task_data:  # if user pressed Escape
                return

            task_name, task_timestamp, task_tag = task_data

            new_c_task = proc.Task(
                name=task_name.encode("utf-8"),
                deadline=task_timestamp,
                tag=task_tag,
            )

            # backend processing
            proc.backend.save_task(new_c_task)
            proc.backend.Insert(new_c_task, proc.queue)
            proc.backend.push(proc.stack, new_c_task)

            vertical_scroll: VerticalScroll = self.query_one("#vert", VerticalScroll)
            task_widgets = vertical_scroll.query(Task)
            new_Task: Task = Task(new_c_task)

            # Ensuring the new task is placed correctly
            inserted: bool = False
            for widget in task_widgets:
                if new_Task.deadline < widget.deadline:
                    vertical_scroll.mount(new_Task, before=widget)
                    inserted = True
                    break

            if not inserted:
                vertical_scroll.mount(new_Task)

            self.notify(f'Added "{task_name}" successfully!')

        self.push_screen(TaskInputScreen(1), check_input)

    def action_undo_task(self) -> None:
        popped: proc.Task = proc.backend.pop(proc.stack)
        popped_name: str = popped.name.decode(
            "utf-8", errors="ignore"
        ).strip()  # Bytes into String

        vertical_scroll: VerticalScroll = self.query_one("#vert", VerticalScroll)
        task_widgets = vertical_scroll.query(Task)

        widget_removed: bool = False
        i: int = 0
        try:
            while not widget_removed:
                if task_widgets[i].task_name == popped_name:
                    task_widgets[i].remove()
                    widget_removed = True
                    break
                i += 1

            if widget_removed:
                self.notify(f"Removed '{popped_name}")

            else:
                self.notify(f"Couldn't remove '{popped_name}'", severity="error")

        except IndexError:
            self.notify("Stack is empty", severity="error")

        proc.backend.remove_task(popped)
        proc.backend.Deletion(proc.queue, popped)

    def action_tag_search(self) -> None:
        def processing_tags(to_search: str | None):

            if not to_search:
                return
            else:
                to_search: int = int(to_search.strip())

            vertical_scroll: VerticalScroll = self.query_one("#vert", VerticalScroll)
            task_widgets = vertical_scroll.query(Task)

            found_any = False
            for widget in task_widgets:
                if widget.tag == to_search:
                    found_any = True
                    widget.display = True

                else:
                    widget.display = False

            if found_any:
                self.notify(f"Showing tasks with tag {to_search}")
            else:
                self.notify(f"No tasks found with tag {to_search}", severity="error")

        self.push_screen(TaskInputScreen(2), processing_tags)

    def action_reveal(self) -> None:
        vertical_scroll: VerticalScroll = self.query_one("#vert", VerticalScroll)

        for widget in vertical_scroll.children:
            widget.display = True

    def action_exit(self) -> None:
        proc.free_queue_stack()
        self.exit()

    def action_toggle_dark(self) -> None:
        self.theme = (
            "textual-dark" if self.theme == "textual-light" else "textual-light"
        )


if __name__ == "__main__":
    app = SystemC()
    app.run()
