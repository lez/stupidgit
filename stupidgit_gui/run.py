#!/usr/bin/env python
# -*- coding: utf-8

import sys
import os
import os.path
import wx

from git import Repository
from MainWindow import *
from PasswordDialog import *

def main_normal():
    # Parse arguments
    repodir = sys.argv[1] if len(sys.argv) > 1 else os.getcwd()

    # Show main window
    try:
        repo = Repository(repodir)
    except GitError:
        repo = None
    app = wx.PySimpleApp()
    win = MainWindow(repo)
    win.Show(True)
    app.MainLoop()

def main_askpass():
    app = wx.PySimpleApp()

    askpass = PasswordDialog(None, -1, ' '.join(sys.argv[1:]))
    askpass.ShowModal()

    if askpass.password:
        print askpass.password
        sys.exit(0)
    else:
        sys.exit(1)

def main():
    if 'askpass' in sys.argv[0]:
        main_askpass()
    else:
        main_normal()

if __name__ == '__main__':
    main()
