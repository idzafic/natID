Podešenja za QtCreator:
Aktivirati meni opciju Edit->Preferences
U listi sa lijeve strane odabrati Debugger
U polje Extra Debugging Helper unijeti lokaciju odgovrajućeg py modula:
Za linux:
/home/idzafic/Work/DevEnv/LLDB/tdQtCreator_unix.py
Za windows
C:/Users/idzafic/Work/DevEnv/LLDB/tdQtCreator_win.py
Podesiti odgovarajući home folder u putevima iznad ili kliknuti na Browse i odabrati odgovarajući py module

-------------------------------
Podepenja za XCode na macOS

# This file contains commands that LLDB will execute at startup. Drop it in your
# home directory: ~/.lldbinit

1. Uzeti Sadržaj iz lldbinit i prebaciti ga u .lldbinit u home folderu

Odnosno upiši u ~/.lldbinit slijedeći text:
command script import ~/Work/DevEnv/LLDB/tdModule.py
 
# Dump the UIKit view hierarchy
command regex rd 's/[[:space:]]*$/po [[[UIApplication sharedApplication] keyWindow] recursiveDescription]/' 's/.+$/po [%1 recursiveDescription]/'
 
# Try not to step into uninteresting library code. Needs some work.
settings set target.process.thread.step-avoid-regexp ^(std::(!function)|boost::shared_ptr)
 
# https://github.com/facebook/chisel
command script import /usr/local/opt/chisel/libexec/fblldb.py
 
# Load FiftyThree type summaries and command aliases
command script import /Users/brian/src/Paper/Utilities/xcode/lldb.py

