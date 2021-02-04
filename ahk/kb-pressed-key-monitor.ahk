; http://sl5.it/SL5_preg_contentFinder/examples/AutoHotKey/converts_your_autohotkey_code_into_pretty_indented_source_code.php

#NoEnv
#SingleInstance force
#InstallKeybdHook

Global KeyStates, MouseState, ClickCount, IdleDelay, LargeDisplay

IdleDelay = 500

LargeDisplay = 1
; 0 = Tooltip near mouse pointer
; 1 = Big pane at screen bottom

If LargeDisplay
{
   ;Initializing GUI
   ;modded func originated from http://www.autohotkey.com/board/topic/8190-osd-function/

   SysGet Monitor, Monitor

   GUIx := MonitorLeft
   GUIw := MonitorRight - MonitorLeft
   GUIh := (MonitorBottom - MonitorTop) * GUIw * 0.00003
   If (GUIh > ((MonitorBottom - MonitorTop) * 0.3))
   GUIh := (MonitorBottom - MonitorTop) * 0.3

   opacity = 230
   fname   = "Courier New"
   fsize  := GUIh * 0.35 ; really, pixel = 0.75 point, but with 0.75 lowercase letter with lower part (like "g") get cut
   fcolor  = cccccc
   bcolor  = 222222
   fformat = "600"

   Gui +LastFound +AlwaysOnTop +ToolWindow -Caption
   Gui Margin, 0, 0 ;pixels of space to leave at the left/right and top/bottom sides of the window when auto-positioning.
   Gui Color, ffffff ;changes background color
   Gui Font, s%fsize% w%fformat%, %fname%

   ; 0x80 = SS_NOPREFIX -> Ampersand (&) is shown instead of underline one letter for Alt+letter navigation
   Gui Add, Text, c%bcolor% Center +0x80 w%GUIw% h%GUIh% BackgroundTrans VblkOsdCtrlName, tesT test test
   ; Gui Add, Text, c%fcolor% Center +0x80 w%GUIw% h%GUIh% BackgroundTrans VblkOsdCtrlName2 xp-3 yp-3 , tesT test test
   Gui Add, Text, c%fcolor% Center +0x80 w%GUIw% h%GUIh% BackgroundTrans VblkOsdCtrlName2 xp-2 yp-2, tesT test test

   WinSet ExStyle, +0x20 ; WS_EX_TRANSPARENT -> mouse klickthrough
   WinSet TransColor, ffffff %opacity%
}

TrayTip %A_ScriptName%, To Exit`, press the Right Windows logo key.

Loop
{
   Input SingleKey, L1 V M I B, {LControl}{RControl}{LAlt}{RAlt}{LShift}{RShift}{LWin}{AppsKey}{F1}{F2}{F3}{F4}{F5}{F6}{F7}{F8}{F9}{F10}{F11}{F12}{Left}{Right}{Up}{Down}{Home}{End}{PgUp}{PgDn}{Del}{Ins}{BS}{Capslock}{Numlock}{PrintScreen}{Pause}
   GetKeyStates()
   ;   IfInString ErrorLevel, EndKey
   ;   CollectedText .= (CollectedText =="" ? "" : " + " ) . SubStr(ErrorLevel, 8)
   If SingleKey
   {
      SingleKeyText=
      If (SingleKey == Chr(27))
      {
         SingleKeyText=Esc
      }
      Else If (SingleKey == Chr(32))
      {
         SingleKeyText=Space
      }

      If (GetKeyState("LControl", "P") || GetKeyState("RControl", "P"))
      {
         If (SingleKey == Chr(10) && EnterPressed)
            SingleKeyText = Enter
         Else If (SingleKey >= Chr(1) && SingleKey <= Chr(26)) ; With "M" in Input, Ctrl-A through Ctrl-Z correspond to Chr(1) through Chr(26)
            SingleKeyText := Chr(Asc("A")-1+Asc(SingleKey))
      }
      else
      {
         If (SingleKey == Chr(10))
            SingleKeyText=Enter
      }

      If Not SingleKeyText
      SingleKeyText := SingleKey
      ShowKeys(KeyStates . SingleKeyText)
      ; . (MouseState ? "`n" . MouseState : "")
      EnterPressed=0
      ; GoSub ClearMouseState
   }
   else
   {
      ShowKeys(SubStr(KeyStates, 1, -3) . (MouseState ? " + " . MouseState : ""))
   }

   SetTimer TooltipOff, % -IdleDelay
}

RWin::
   ExitApp

   ~*Enter::
   EnterPressed=1
return

; ~*LButton::
   ; ~*RButton::
   ; ~*MButton::
   ; ~*XButton1::
   ; ~*XButton2::
   ; MouseState := "Mouse " . SubStr(A_ThisHotkey, 3) . " Pressed"
   ; MouseTooltip()
   ; return

   ; ~*WheelDown::
   ; ~*WheelUp::
   ; ~*WheelLeft::
   ; ~*WheelRight::
   ; MouseState := "Mouse " . SubStr(A_ThisHotkey, 3)
   ; MouseTooltip()
   ; return

   ; ~*LButton Up::
   ; ~*RButton Up::
   ; ~*MButton Up::
   ; ~*XButton1 Up::
   ; ~*XButton2 Up::
   ; ;    MsgBox %A_PriorHotkey%`n%A_ThisHotkey%
   ; If (A_PriorHotkey == SubStr(A_ThisHotkey, 1, -3) && A_TimeSincePriorHotkey < 200)
   ; {
   ;     ClickCount++
   ;     Suffix := " Clicked " . ClickCount . "x"
   ; }Else {
   ;     ClickCount:=0
   ;     Suffix := " Released"
   ; }
   ; MouseState := "Mouse " . SubStr(A_ThisHotkey, 3, -3) . Suffix
   ; MouseTooltip()
   ; return

   ; MouseTooltip(){
   ; GetKeyStates()
   ; ShowKeys(KeyStates . MouseState)
   ; SetTimer ClearMouseState,  % -IdleDelay
   ; SetTimer TooltipOff, % -IdleDelay
   ; }

   ; ClearMouseState:
   ; MouseState=
   ; ClickCount=0
   ; return

   TooltipOff:
   If LargeDisplay
      Gui Hide
   Else
      Tooltip
return

GetKeyStates()
{
   KeyStates := ""
   . ( GetKeyState("LControl", "P") ? "LControl + " : "" )
   . ( GetKeyState("RControl", "P") ? "RControl + " : "" )
   . ( GetKeyState("LAlt", "P") ? "LAlt + " : "" )
   . ( GetKeyState("RAlt", "P") ? "RAlt + " : "" )
   . ( GetKeyState("LShift", "P") ? "LShift + " : "" )
   . ( GetKeyState("RShift", "P") ? "RShift + " : "" )
   . ( GetKeyState("LWin", "P") ? "LWin + " : "" )
   . ( GetKeyState("AppsKey", "P") ? "AppsKey + " : "" )
   . ( GetKeyState("F1", "P") ? "F1 + " : "" )
   . ( GetKeyState("F2", "P") ? "F2 + " : "" )
   . ( GetKeyState("F3", "P") ? "F3 + " : "" )
   . ( GetKeyState("F4", "P") ? "F4 + " : "" )
   . ( GetKeyState("F5", "P") ? "F5 + " : "" )
   . ( GetKeyState("F6", "P") ? "F6 + " : "" )
   . ( GetKeyState("F7", "P") ? "F7 + " : "" )
   . ( GetKeyState("F8", "P") ? "F8 + " : "" )
   . ( GetKeyState("F9", "P") ? "F9 + " : "" )
   . ( GetKeyState("F10", "P") ? "F10 + " : "" )
   . ( GetKeyState("F11", "P") ? "F11 + " : "" )
   . ( GetKeyState("F12", "P") ? "F12 + " : "" )
   . ( GetKeyState("Left", "P") ? "Left + " : "" )
   . ( GetKeyState("Right", "P") ? "Right + " : "" )
   . ( GetKeyState("Up", "P") ? "Up + " : "" )
   . ( GetKeyState("Down", "P") ? "Down + " : "" )
   . ( GetKeyState("Home", "P") ? "Home + " : "" )
   . ( GetKeyState("End", "P") ? "End + " : "" )x
   . ( GetKeyState("PgUp", "P") ? "PgUp + " : "" )
   . ( GetKeyState("PgDn", "P") ? "PgDn + " : "" )
   . ( GetKeyState("Del", "P") ? "Del + " : "" )
   . ( GetKeyState("Ins", "P") ? "Ins + " : "" )
   . ( GetKeyState("Backspace", "P") ? "Backspace + " : "" )
   . ( GetKeyState("Capslock", "P") ? "Capslock + " : "" )
   . ( GetKeyState("Numlock", "P") ? "Numlock + " : "" )
   . ( GetKeyState("PrintScreen", "P") ? "PrintScreen + " : "" )
   . ( GetKeyState("Pause", "P") ? "Pause + " : "" )
}

ShowKeys(text)
{
   If LargeDisplay
   {
      Global blkOsdCtrlName, blkOsdCtrlName2, MonitorLeft, MonitorRight, MonitorBottom, MonitorTop, GUIx, GUIy, GUIh

      CoordMode Mouse, Screen
      MouseGetPos MouseX, MouseY

      If ((!GUIy) || (MouseX >= MonitorLeft && MouseX <= MonitorRight && MouseY >= GUIy && MouseY <= (GUIy + GUIh)) )
      {
         GUIy := MonitorTop + (MonitorBottom - MonitorTop) * 0.8
         ; If (MouseY < (MonitorTop + (MonitorBottom - MonitorTop) / 2))
         ;    GUIy := MonitorBottom - (MonitorBottom - MonitorTop) * 0.2
         ; Else
         ;    GUIy := MonitorTop + (MonitorBottom - MonitorTop) * 0.2
      }

      GuiControl Text, blkOsdCtrlName, %text%
      GuiControl Text, blkOsdCtrlName2, %text%

      Gui, Show, x%GUIx% y%GUIy% NoActivate
   }
   else
   {
      Tooltip % text
   }
}