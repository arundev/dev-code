namespace WorldEditor.Common.Common
{
    // Win32的一些枚举变量
    public partial class Win32
    {
        // Window消息,来自WinUser.h
        public enum WinMsg : int
        {
            WM_NULL = 0x0000,
            WM_CREATE = 0x0001,
            WM_DESTROY = 0x0002,
            WM_MOVE = 0x0003,
            WM_SIZE = 0x0005,
            WM_ACTIVATE = 0x0006,


            //        WM_SETFOCUS                     0x0007
            //        WM_KILLFOCUS                    0x0008
            //        WM_ENABLE                       0x000A
            //        WM_SETREDRAW                    0x000B
            //        WM_SETTEXT                      0x000C
            //        WM_GETTEXT                      0x000D
            //        WM_GETTEXTLENGTH                0x000E
            //        WM_PAINT                        0x000F
            //        WM_CLOSE                        0x0010
            //        WM_QUERYENDSESSION              0x0011
            //        WM_QUIT                         0x0012
            //        WM_QUERYOPEN                    0x0013
            //        WM_ERASEBKGND                   0x0014
            //        WM_SYSCOLORCHANGE               0x0015
            //        WM_ENDSESSION                   0x0016
            //        WM_SHOWWINDOW                   0x0018
            //        WM_WININICHANGE                 0x001A
            //        WM_DEVMODECHANGE                0x001B
            WM_ACTIVATEAPP = 0x001C,
            //        WM_FONTCHANGE                   0x001D
            //        WM_TIMECHANGE                   0x001E
            //        WM_CANCELMODE                   0x001F
            //        WM_SETCURSOR                    0x0020
            //        WM_MOUSEACTIVATE                0x0021
            //        WM_CHILDACTIVATE                0x0022
            //        WM_QUEUESYNC                    0x0023
            //        WM_GETMINMAXINFO                0x0024
            //        WM_PAINTICON                    0x0026
            //        WM_ICONERASEBKGND               0x0027
            //        WM_NEXTDLGCTL                   0x0028
            //        WM_SPOOLERSTATUS                0x002A
            //        WM_DRAWITEM                     0x002B
            //        WM_MEASUREITEM                  0x002C
            //        WM_DELETEITEM                   0x002D
            //        WM_VKEYTOITEM                   0x002E
            //        WM_CHARTOITEM                   0x002F
            //        WM_SETFONT                      0x0030
            //        WM_GETFONT                      0x0031
            //#define WM_SETHOTKEY                    0x0032
            //#define WM_GETHOTKEY                    0x0033
            //#define WM_QUERYDRAGICON                0x0037
            //#define WM_COMPAREITEM                  0x0039
            //#if(WINVER >= 0x0500)
            //#ifndef _WIN32_WCE
            //#define WM_GETOBJECT                    0x003D
            //#endif
            //#endif /* WINVER >= 0x0500 */
            //#define WM_COMPACTING                   0x0041
            //#define WM_COMMNOTIFY                   0x0044  /* no longer suported */
            //#define WM_WINDOWPOSCHANGING            0x0046
            //#define WM_WINDOWPOSCHANGED             0x0047

            //#define WM_POWER                        0x0048
            ///*
            // * wParam for WM_POWER window message and DRV_POWER driver notification
            // */
            //#define PWR_OK              1
            //#define PWR_FAIL            (-1)
            //#define PWR_SUSPENDREQUEST  1
            //#define PWR_SUSPENDRESUME   2
            //#define PWR_CRITICALRESUME  3

            //#define WM_COPYDATA                     0x004A
            //#define WM_CANCELJOURNAL                0x004B


            ///*
            // * lParam of WM_COPYDATA message points to...
            // */
            //typedef struct tagCOPYDATASTRUCT {
            //    ULONG_PTR dwData;
            //    DWORD cbData;
            //    __field_bcount(cbData) PVOID lpData;
            //} COPYDATASTRUCT, *PCOPYDATASTRUCT;

            //#if(WINVER >= 0x0400)
            //typedef struct tagMDINEXTMENU
            //{
            //    HMENU   hmenuIn;
            //    HMENU   hmenuNext;
            //    HWND    hwndNext;
            //} MDINEXTMENU, * PMDINEXTMENU, FAR * LPMDINEXTMENU;
            //#endif /* WINVER >= 0x0400 */


            //#if(WINVER >= 0x0400)
            //#define WM_NOTIFY                       0x004E
            //#define WM_INPUTLANGCHANGEREQUEST       0x0050
            //#define WM_INPUTLANGCHANGE              0x0051
            //#define WM_TCARD                        0x0052
            //#define WM_HELP                         0x0053
            //#define WM_USERCHANGED                  0x0054
            //#define WM_NOTIFYFORMAT                 0x0055

            //#define NFR_ANSI                             1
            //#define NFR_UNICODE                          2
            //#define NF_QUERY                             3
            //#define NF_REQUERY                           4

            //#define WM_CONTEXTMENU                  0x007B
            //#define WM_STYLECHANGING                0x007C
            //#define WM_STYLECHANGED                 0x007D
            //#define WM_DISPLAYCHANGE                0x007E
            //#define WM_GETICON                      0x007F
            //#define WM_SETICON                      0x0080
            //#endif /* WINVER >= 0x0400 */

            //#define WM_NCCREATE                     0x0081
            //#define WM_NCDESTROY                    0x0082
            //#define WM_NCCALCSIZE                   0x0083
            //#define WM_NCHITTEST                    0x0084
            //#define WM_NCPAINT                      0x0085
            //#define WM_NCACTIVATE                   0x0086
            //#define WM_GETDLGCODE                   0x0087
            //#ifndef _WIN32_WCE
            //#define WM_SYNCPAINT                    0x0088
            //#endif
            //#define WM_NCMOUSEMOVE                  0x00A0
            //#define WM_NCLBUTTONDOWN                0x00A1
            //#define WM_NCLBUTTONUP                  0x00A2
            //#define WM_NCLBUTTONDBLCLK              0x00A3
            //#define WM_NCRBUTTONDOWN                0x00A4
            //#define WM_NCRBUTTONUP                  0x00A5
            //#define WM_NCRBUTTONDBLCLK              0x00A6
            //#define WM_NCMBUTTONDOWN                0x00A7
            //#define WM_NCMBUTTONUP                  0x00A8
            //#define WM_NCMBUTTONDBLCLK              0x00A9



            //#if(_WIN32_WINNT >= 0x0500)
            //#define WM_NCXBUTTONDOWN                0x00AB
            //#define WM_NCXBUTTONUP                  0x00AC
            //#define WM_NCXBUTTONDBLCLK              0x00AD
            //#endif /* _WIN32_WINNT >= 0x0500 */


            //#if(_WIN32_WINNT >= 0x0501)
            //#define WM_INPUT_DEVICE_CHANGE          0x00FE
            //#endif /* _WIN32_WINNT >= 0x0501 */

            //#if(_WIN32_WINNT >= 0x0501)
            //#define WM_INPUT                        0x00FF
            //#endif /* _WIN32_WINNT >= 0x0501 */

            //#define WM_KEYFIRST                     0x0100
            //#define WM_KEYDOWN                      0x0100
            //#define WM_KEYUP                        0x0101
            //#define WM_CHAR                         0x0102
            //#define WM_DEADCHAR                     0x0103
            //#define WM_SYSKEYDOWN                   0x0104
            //#define WM_SYSKEYUP                     0x0105
            //#define WM_SYSCHAR                      0x0106
            //#define WM_SYSDEADCHAR                  0x0107
            //#if(_WIN32_WINNT >= 0x0501)
            //#define WM_UNICHAR                      0x0109
            //#define WM_KEYLAST                      0x0109
            //#define UNICODE_NOCHAR                  0xFFFF
            //#else
            //#define WM_KEYLAST                      0x0108
            //#endif /* _WIN32_WINNT >= 0x0501 */

            //#if(WINVER >= 0x0400)
            //#define WM_IME_STARTCOMPOSITION         0x010D
            //#define WM_IME_ENDCOMPOSITION           0x010E
            //#define WM_IME_COMPOSITION              0x010F
            //#define WM_IME_KEYLAST                  0x010F
            //#endif /* WINVER >= 0x0400 */

            //#define WM_INITDIALOG                   0x0110
            //#define WM_COMMAND                      0x0111
            //#define WM_SYSCOMMAND                   0x0112
            //#define WM_TIMER                        0x0113
            //#define WM_HSCROLL                      0x0114
            //#define WM_VSCROLL                      0x0115
            //#define WM_INITMENU                     0x0116
            //#define WM_INITMENUPOPUP                0x0117
            //#define WM_MENUSELECT                   0x011F
            //#define WM_MENUCHAR                     0x0120
            //#define WM_ENTERIDLE                    0x0121
            //#if(WINVER >= 0x0500)
            //#ifndef _WIN32_WCE
            //#define WM_MENURBUTTONUP                0x0122
            //#define WM_MENUDRAG                     0x0123
            //#define WM_MENUGETOBJECT                0x0124
            //#define WM_UNINITMENUPOPUP              0x0125
            //#define WM_MENUCOMMAND                  0x0126

            //#ifndef _WIN32_WCE
            //#if(_WIN32_WINNT >= 0x0500)
            //#define WM_CHANGEUISTATE                0x0127
            //#define WM_UPDATEUISTATE                0x0128
            //#define WM_QUERYUISTATE                 0x0129

            ///*
            // * LOWORD(wParam) values in WM_*UISTATE*
            // */
            //#define UIS_SET                         1
            //#define UIS_CLEAR                       2
            //#define UIS_INITIALIZE                  3

            ///*
            // * HIWORD(wParam) values in WM_*UISTATE*
            // */
            //#define UISF_HIDEFOCUS                  0x1
            //#define UISF_HIDEACCEL                  0x2
            //#if(_WIN32_WINNT >= 0x0501)
            //#define UISF_ACTIVE                     0x4
            //#endif /* _WIN32_WINNT >= 0x0501 */
            //#endif /* _WIN32_WINNT >= 0x0500 */
            //#endif

            //#endif
            //#endif /* WINVER >= 0x0500 */

            //#define WM_CTLCOLORMSGBOX               0x0132
            //#define WM_CTLCOLOREDIT                 0x0133
            //#define WM_CTLCOLORLISTBOX              0x0134
            //#define WM_CTLCOLORBTN                  0x0135
            //#define WM_CTLCOLORDLG                  0x0136
            //#define WM_CTLCOLORSCROLLBAR            0x0137
            //#define WM_CTLCOLORSTATIC               0x0138
            //#define MN_GETHMENU                     0x01E1

            WM_MOUSEFIRST = 0x0200,
            WM_MOUSEMOVE = 0x0200,
            WM_LBUTTONDOWN = 0x0201,
            WM_LBUTTONUP = 0x0202,
            WM_LBUTTONDBLCLK = 0x0203,
            WM_RBUTTONDOWN = 0x0204,
            WM_RBUTTONUP = 0x0205,
            WM_RBUTTONDBLCLK = 0x0206,
            WM_MBUTTONDOWN = 0x0207,
            WM_MBUTTONUP = 0x0208,
            WM_MBUTTONDBLCLK = 0x0209,
            //#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
            //#define WM_MOUSEWHEEL                   0x020A
            //#endif
            //#if (_WIN32_WINNT >= 0x0500)
            //#define WM_XBUTTONDOWN                  0x020B
            //#define WM_XBUTTONUP                    0x020C
            //#define WM_XBUTTONDBLCLK                0x020D
            //#endif
            //#if (_WIN32_WINNT >= 0x0600)
            //#define WM_MOUSEHWHEEL                  0x020E
            //#endif

            //#if (_WIN32_WINNT >= 0x0600)
            //#define WM_MOUSELAST                    0x020E
            //#elif (_WIN32_WINNT >= 0x0500)
            //#define WM_MOUSELAST                    0x020D
            //#elif (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
            //#define WM_MOUSELAST                    0x020A
            //#else
            //#define WM_MOUSELAST                    0x0209
            //#endif /* (_WIN32_WINNT >= 0x0600) */


            //#if(_WIN32_WINNT >= 0x0400)
            ///* Value for rolling one detent */
            //#define WHEEL_DELTA                     120
            //#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))

            ///* Setting to scroll one page for SPI_GET/SETWHEELSCROLLLINES */
            //#define WHEEL_PAGESCROLL                (UINT_MAX)
            //#endif /* _WIN32_WINNT >= 0x0400 */

            //#if(_WIN32_WINNT >= 0x0500)
            //#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))
            //#define GET_NCHITTEST_WPARAM(wParam)    ((short)LOWORD(wParam))
            //#define GET_XBUTTON_WPARAM(wParam)      (HIWORD(wParam))

            ///* XButton values are WORD flags */
            //#define XBUTTON1      0x0001
            //#define XBUTTON2      0x0002
            ///* Were there to be an XBUTTON3, its value would be 0x0004 */
            //#endif /* _WIN32_WINNT >= 0x0500 */

            //#define WM_PARENTNOTIFY                 0x0210
            //#define WM_ENTERMENULOOP                0x0211
            //#define WM_EXITMENULOOP                 0x0212

            //#if(WINVER >= 0x0400)
            //#define WM_NEXTMENU                     0x0213
            //#define WM_SIZING                       0x0214
            //#define WM_CAPTURECHANGED               0x0215
            //#define WM_MOVING                       0x0216
            //#endif /* WINVER >= 0x0400 */

            //#if(WINVER >= 0x0400)


            //#define WM_POWERBROADCAST               0x0218

            //#ifndef _WIN32_WCE
            //#define PBT_APMQUERYSUSPEND             0x0000
            //#define PBT_APMQUERYSTANDBY             0x0001

            //#define PBT_APMQUERYSUSPENDFAILED       0x0002
            //#define PBT_APMQUERYSTANDBYFAILED       0x0003

            //#define PBT_APMSUSPEND                  0x0004
            //#define PBT_APMSTANDBY                  0x0005

            //#define PBT_APMRESUMECRITICAL           0x0006
            //#define PBT_APMRESUMESUSPEND            0x0007
            //#define PBT_APMRESUMESTANDBY            0x0008

            //#define PBTF_APMRESUMEFROMFAILURE       0x00000001

            //#define PBT_APMBATTERYLOW               0x0009
            //#define PBT_APMPOWERSTATUSCHANGE        0x000A

            //#define PBT_APMOEMEVENT                 0x000B


            //#define PBT_APMRESUMEAUTOMATIC          0x0012
            //#if (_WIN32_WINNT >= 0x0502)
            //#ifndef PBT_POWERSETTINGCHANGE
            //#define PBT_POWERSETTINGCHANGE          0x8013
            //typedef struct {
            //    GUID PowerSetting;
            //    DWORD DataLength;
            //    UCHAR Data[1];
            //} POWERBROADCAST_SETTING, *PPOWERBROADCAST_SETTING;


            //#endif // PBT_POWERSETTINGCHANGE

            //#endif // (_WIN32_WINNT >= 0x0502)
            //#endif

            //#endif /* WINVER >= 0x0400 */

            //#if(WINVER >= 0x0400)
            //#define WM_DEVICECHANGE                 0x0219
            //#endif /* WINVER >= 0x0400 */

            //#define WM_MDICREATE                    0x0220
            //#define WM_MDIDESTROY                   0x0221
            //#define WM_MDIACTIVATE                  0x0222
            //#define WM_MDIRESTORE                   0x0223
            //#define WM_MDINEXT                      0x0224
            //#define WM_MDIMAXIMIZE                  0x0225
            //#define WM_MDITILE                      0x0226
            //#define WM_MDICASCADE                   0x0227
            //#define WM_MDIICONARRANGE               0x0228
            //#define WM_MDIGETACTIVE                 0x0229


            //#define WM_MDISETMENU                   0x0230
            //#define WM_ENTERSIZEMOVE                0x0231
            //#define WM_EXITSIZEMOVE                 0x0232
            //#define WM_DROPFILES                    0x0233
            //#define WM_MDIREFRESHMENU               0x0234


            //#if(WINVER >= 0x0400)
            //#define WM_IME_SETCONTEXT               0x0281
            //#define WM_IME_NOTIFY                   0x0282
            //#define WM_IME_CONTROL                  0x0283
            //#define WM_IME_COMPOSITIONFULL          0x0284
            //#define WM_IME_SELECT                   0x0285
            //#define WM_IME_CHAR                     0x0286
            //#endif /* WINVER >= 0x0400 */
            //#if(WINVER >= 0x0500)
            //#define WM_IME_REQUEST                  0x0288
            //#endif /* WINVER >= 0x0500 */
            //#if(WINVER >= 0x0400)
            //#define WM_IME_KEYDOWN                  0x0290
            //#define WM_IME_KEYUP                    0x0291
            //#endif /* WINVER >= 0x0400 */

            //#if((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
            //#define WM_MOUSEHOVER                   0x02A1
            //#define WM_MOUSELEAVE                   0x02A3
            //#endif
            //#if(WINVER >= 0x0500)
            //#define WM_NCMOUSEHOVER                 0x02A0
            //#define WM_NCMOUSELEAVE                 0x02A2
            //#endif /* WINVER >= 0x0500 */

            //#if(_WIN32_WINNT >= 0x0501)
            //#define WM_WTSSESSION_CHANGE            0x02B1

            //#define WM_TABLET_FIRST                 0x02c0
            //#define WM_TABLET_LAST                  0x02df
            //#endif /* _WIN32_WINNT >= 0x0501 */

            //#define WM_CUT                          0x0300
            //#define WM_COPY                         0x0301
            //#define WM_PASTE                        0x0302
            //#define WM_CLEAR                        0x0303
            //#define WM_UNDO                         0x0304
            //#define WM_RENDERFORMAT                 0x0305
            //#define WM_RENDERALLFORMATS             0x0306
            //#define WM_DESTROYCLIPBOARD             0x0307
            //#define WM_DRAWCLIPBOARD                0x0308
            //#define WM_PAINTCLIPBOARD               0x0309
            //#define WM_VSCROLLCLIPBOARD             0x030A
            //#define WM_SIZECLIPBOARD                0x030B
            //#define WM_ASKCBFORMATNAME              0x030C
            //#define WM_CHANGECBCHAIN                0x030D
            //#define WM_HSCROLLCLIPBOARD             0x030E
            //#define WM_QUERYNEWPALETTE              0x030F
            //#define WM_PALETTEISCHANGING            0x0310
            //#define WM_PALETTECHANGED               0x0311
            //#define WM_HOTKEY                       0x0312

            //#if(WINVER >= 0x0400)
            //#define WM_PRINT                        0x0317
            //#define WM_PRINTCLIENT                  0x0318
            //#endif /* WINVER >= 0x0400 */

            //#if(_WIN32_WINNT >= 0x0500)
            //#define WM_APPCOMMAND                   0x0319
            //#endif /* _WIN32_WINNT >= 0x0500 */

            //#if(_WIN32_WINNT >= 0x0501)
            //#define WM_THEMECHANGED                 0x031A
            //#endif /* _WIN32_WINNT >= 0x0501 */


            //#if(_WIN32_WINNT >= 0x0501)
            //#define WM_CLIPBOARDUPDATE              0x031D
            //#endif /* _WIN32_WINNT >= 0x0501 */

            //#if(_WIN32_WINNT >= 0x0600)
            //#define WM_DWMCOMPOSITIONCHANGED        0x031E
            //#define WM_DWMNCRENDERINGCHANGED        0x031F
            //#define WM_DWMCOLORIZATIONCOLORCHANGED  0x0320
            //#define WM_DWMWINDOWMAXIMIZEDCHANGE     0x0321
            //#endif /* _WIN32_WINNT >= 0x0600 */

            //#if(WINVER >= 0x0600)
            //#define WM_GETTITLEBARINFOEX            0x033F
            //        WM_HANDHELDFIRST                0x0358
            //        WM_HANDHELDLAST                 0x035F

            //        WM_AFXFIRST     = 0x0360,
            //        WM_AFXLAST      = 0x037F,
            //        WM_PENWINFIRST  = 0x0380,
            //        WM_PENWINLAST   = 0x038F,
            //        WM_APP          = 0x8000,
            //        WM_USER         = 0x0400,             // All Message Numbers below 0x0400 are RESERVED.  
            //        WM_REFLECT      = WM_USER + 0x1c00,
        }

        // Window窗口样式
        public enum WindowStyle : uint
        {

        }

        // Window扩展窗口样式
        public enum WIndowExStyle : uint
        {

        }

        //
    }
}