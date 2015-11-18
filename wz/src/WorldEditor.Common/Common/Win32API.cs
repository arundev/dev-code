using System;
using System.Runtime.InteropServices;

namespace WorldEditor.Common.Common
{
    // Win32的一些API函数
    public partial class Win32
    {
        // user32.dll
        // @{
        // 获得当前活动窗体的句柄
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern System.IntPtr GetForegroundWindow();

        // 设置此窗体句柄的窗体为活动窗体
        [DllImport("user32.dll", EntryPoint = "SetForegroundWindow")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);
        // @}
    }

}