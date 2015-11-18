using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices; 

namespace WorldEditor
{
    //*****************************************************************************************
    // Win32API导入函数与数据结构
    //
    //
    //*****************************************************************************************
    class NativeMethod
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct Message
        {
            public IntPtr hWnd;
            public IntPtr hMsg;
            public IntPtr wParam;
            public IntPtr lParam;
            public uint time;
            public System.Drawing.Point p;
        }

        [System.Security.SuppressUnmanagedCodeSecurity]
        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        public static extern bool PeekMessage(out Message msg, IntPtr hWnd, uint messageFilterMin, uint messageFilterMax, uint flags);
    }
}