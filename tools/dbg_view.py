from textual import messages
from textual.app import App, ComposeResult, Region
from textual.screen import Screen
from textual.containers import Horizontal
from textual.widgets import Header, Footer, Button, RichLog, Static, Input, Label, RadioButton

from tkinter import filedialog
from tkinter import Tk

import os

FILEPATH = 'C:\\devkitPro\\examples\\3ds\\C2D_ABS\\a.txt'


class Form(Screen):
    #CSS_PATH = "res\\dbg_view1.tcss"

    richlog: RichLog
    
    can_update = True
    
    BINDINGS = [
        ("D", "stop", "Toggle updating the view"),
        ("Q", "quit", "Quit the app")
    ]

    def compose(self) -> ComposeResult:
        self.title = "Real-time file viewer"

        yield Header(show_clock=True)
        yield Footer()

        yield RichLog(highlight=True, markup=True, id="richlog")
        
        
    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Event handler called when a button is pressed."""
        if event.button.id == "button-filedialog":
            Tk().withdraw()
            filename = filedialog.askopenfile(initialdir= "/",title="Select file to load...")
            file_input = self.query_one("#input-infile", Input)
            file_input.value = filename
    
    def read_file(self, filename):
        with open(filename, "r") as f:
            return f.readlines()

    def _on_update(self, message: messages.Update) -> None:
        richlog = self.query_one("#richlog", RichLog)
        lines = self.read_file(FILEPATH)
        richlog.clear()
        to_write = ""
        for elem in lines:
            to_write += elem
        richlog.write(to_write)

    def action_stop(self) -> None:
        """An action to toggle dark mode."""
        self.can_update = not self.can_update
     
        
class Main(App):
    BINDING = [
        ("N", "new_attack", "Start viewing file"),
        ("D", "toggle_dark", "Toggle dark mode"),
        ("Q", "quit", "Quit the app")
    ]

    def compose(self) -> ComposeResult:
        """Create child widgets for the app."""
        yield Header(show_clock=True)
        yield Footer()

        yield Button("Start file viewer", variant="default", id="button-filedialog", classes="col button-dialog")
        
    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Event handler called when a button is pressed."""
        if event.button.id == "button-filedialog":
            self.install_screen(Form(), "attack_window")
            self.push_screen("attack_window")
    
    def action_toggle_dark(self) -> None:
        """An action to toggle dark mode."""
        self.dark = not self.dark

    def action_new_attack(self) -> None:
        BINDING = []
        self.install_screen(Form(), "attack_window")
        self.push_screen("attack_window")


if __name__ == "__main__":
    app = Main()
    app.run()