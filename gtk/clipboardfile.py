#!/usr/bin/env python

import gtk, shutil

class ClipboardFile:
    def __init__(self):
        window = gtk.Window()
        window.set_default_size(200, -1)
        
        self.clipboard = gtk.Clipboard()
        
        table = gtk.Table()
        table.set_row_spacings(5)
        table.set_col_spacings(5)
        separator = gtk.HSeparator()
        
        label = gtk.Label("Select file to copy/move")
        filebutton_copy = gtk.FileChooserButton("Select A File", None)
        self.radio_1_copy = gtk.RadioButton(None, "Copy", True)
        self.radio_1_move = gtk.RadioButton(self.radio_1_copy, "Move", True)
        button1 = gtk.Button("copy/cut")

        window.add(table)
        table.attach(label, 0, 4, 0, 1)
        table.attach(filebutton_copy, 0, 1, 1, 2)
        table.attach(self.radio_1_copy, 1, 2, 1, 2)
        table.attach(self.radio_1_move, 2, 3, 1, 2)
        table.attach(button1, 3, 4, 1, 2)
        table.attach(separator, 0, 4, 2, 3)
        
        label = gtk.Label("Select destination for file(s)")
        filebutton_paste = gtk.FileChooserButton("Select A File", None)
        filebutton_paste.set_action(gtk.FILE_CHOOSER_ACTION_SELECT_FOLDER)
        button2 = gtk.Button("paste")
        self.label = label
        
        table.attach(label, 0, 4, 3, 4)
        table.attach(filebutton_paste, 0, 1, 4, 5)
        table.attach(button2, 3, 4, 4, 5)
        
        window.connect("destroy", lambda w: gtk.main_quit())
        button1.connect("clicked", self.copy, filebutton_copy)
        button2.connect("clicked", self.paste, filebutton_paste)
        
        window.show_all()
    
    def copy(self, button, filebutton):
        def get_func(clipboard, selection, info, data):
            information = action + "\n" + source
            print information
            selection.set(selection.get_target(), 8, information)
        
        def clear_func(clipboard, data):
            pass
        
        source = filebutton.get_uri()
        
        if self.radio_1_copy.get_active():
            action = "copy"
        elif self.radio_1_move.get_active():
            action = "cut"
        
        targets = [("x-special/gnome-copied-files", 0, 0), ("text/uri-list", 0, 0)]
        self.clipboard.set_with_data(targets, get_func, clear_func)
    
    def paste(self, button, filebutton):
        def callback(clipboard, selection, data):
            information = selection.data.splitlines()
            
            action = information[0]
            files = information[1:]
            
            for file in files:
                if action == "copy":
                    shutil.copy(file[7:], destination)
                elif action == "move":
                    shutil.move(file[7:], destination)
        
        destination = filebutton.get_filename()
        
        self.clipboard.request_contents("x-special/gnome-copied-files", callback)

ClipboardFile()
gtk.main()
