using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditor.Common.Common;

namespace WorldEditor.Common.CustomWidget
{
    // 调色板部件
    public partial class ColorPaletteUserControl : UserControl
    {
        // 显示方式
        public enum EDrawStyle
        {
            Hue,
            Saturation,
            Luminance,
            Red,
            Green,
            Blue,
        };

        // 当前选中的调色板RGB颜色
        private Color mRGB;
        public Color RGB
        {
            get { return mRGB; }
            set
            {
                mRGB = value;
                mHSL = ColorSpaceConverter.RGB2HSL(mRGB);

                ResetMarker(true);
                DrawControl();
            }
        }

        // 当前选中的调色板HSL颜色
        private ColorSpaceConverter.HSL mHSL;
        public ColorSpaceConverter.HSL HSL
        {
            get { return mHSL; }
            set 
            { 
                mHSL = value;
                mRGB = ColorSpaceConverter.HSL2RGB(mHSL);

                ResetMarker(true);
                DrawControl();
            }
        }

        // 当前调色板显示方式
        EDrawStyle mDrawStyle;
        public EDrawStyle DrawStyle
        {
            get { return mDrawStyle; }
            set
            {
                mDrawStyle = value;

                ResetMarker(true);
                DrawControl();
            }
        }

        // 当前标记显示
        private bool mDragging = false;
        private Point mMarkerPos;

        // 编辑事件
        public new event EventHandler Scroll;

        public ColorPaletteUserControl()
        {
            InitializeComponent();

            mHSL = new ColorSpaceConverter.HSL(1.0, 1.0, 1.0);
            mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
            mDrawStyle = EDrawStyle.Hue;
            mMarkerPos = new Point(0, 0);
        }

        //*********************************************************************
        // 函数重载
        //*********************************************************************
        // 重载背景绘制函数,避免闪烁
        protected override void OnPaintBackground(PaintEventArgs e)
        {

        }

        //*********************************************************************
        // 控件消息响应
        //*********************************************************************
        private void ColorPaletteUserControl_Load(object sender, EventArgs e)
        {
            DrawControl();
        }

        private void ColorPaletteUserControl_Resize(object sender, EventArgs e)
        {
            DrawControl();
        }

        private void ColorPaletteUserControl_Paint(object sender, PaintEventArgs e)
        {
            DrawControl();
        }

        private void ColorPaletteUserControl_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
                return;

            // 设置拖曳标记
            mDragging = true;

            // 计算标记位置
            Point pos = new Point(e.X - 2, e.Y - 2);
            if (pos.X < 0) pos.X = 0;
            if (pos.Y < 0) pos.Y = 0;
            if (pos.X > Width - 4) pos.X = Width - 4;
            if (pos.Y > Height - 4) pos.Y = Height - 4;

            if (mMarkerPos == pos)
                return;

            // 重绘标记
            DrawMarker(pos, true);

            // 获取当前颜色
            ResetColor();

            // 发出颜色更改事件
            if (Scroll != null)
                Scroll(this, e);
        }

        private void ColorPaletteUserControl_MouseMove(object sender, MouseEventArgs e)
        {
            // 检查拖曳标记
            if (!mDragging)
                return;

            // 计算标记位置
            Point pos = new Point(e.X - 2, e.Y - 2);
            if (pos.X < 0) pos.X = 0;
            if (pos.Y < 0) pos.Y = 0;
            if (pos.X > Width - 4) pos.X = Width - 4;
            if (pos.Y > Height - 4) pos.Y = Height - 4;

            if (mMarkerPos == pos)
                return;

            // 重绘标记
            DrawMarker(pos, true);

            // 获取当前颜色
            ResetColor();

            // 发出颜色更改事件
            if (Scroll != null)
                Scroll(this, e);
        }

        private void ColorPaletteUserControl_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
                return;

            // 检查拖曳标记
            if (!mDragging)
                return;
            mDragging = false;

            // 计算标记位置
            Point pos = new Point(e.X - 2, e.Y - 2);
            if (pos.X < 0) pos.X = 0;
            if (pos.Y < 0) pos.Y = 0;
            if (pos.X > Width - 4) pos.X = Width - 4;
            if (pos.Y > Height - 4) pos.Y = Height - 4;

            if (mMarkerPos == pos)
                return;

            // 重绘标记
            DrawMarker(pos, true);

            // 获取当前颜色
            ResetColor();

            // 发出颜色更改事件
            if (Scroll != null)
                Scroll(this, e);
        }

        // 内部函数,绘制面板
        private void DrawControl()
        {
            DrawBorder();
            DrawContent();
        }

        // 内部函数,绘制边框
        private void DrawBorder()
        {
            Graphics g = this.CreateGraphics();

            // To make the control look like Adobe Photoshop's,the border around the control
            // will be gray line on the top and left side. a white line on the bottom and 
            // the right side, and a black rectangle(line) inside the gray/white rectangle
            Pen pencil;

            pencil = new Pen(Color.FromArgb(172, 168, 153));    ///< The same gray color used by Photoshop
            g.DrawLine(pencil, Width - 2, 0, 0, 0);             ///< Draw top line
            g.DrawLine(pencil, 0, 0, 0, Height - 2);            ///< Draw left hand line

            pencil = new Pen(Color.White);
            g.DrawLine(pencil, Width - 1, 0, Width - 1, Height - 1);    ///< Draw right hand line
            g.DrawLine(pencil, Width - 1, Height - 1, 0, Height - 1);   ///< Draw bottom line

            pencil = new Pen(Color.Black);
            g.DrawRectangle(pencil, 1, 1, Width - 3, Height - 3);       ///< Draw inner black rectangle
        }

        // 内部函数,绘制颜色
        private void DrawContent()
        {
            switch (mDrawStyle)
            {
                case EDrawStyle.Hue :
                    DrawContent_Hue();
                    break;
                case EDrawStyle.Saturation:
                    DrawContent_Saturation();
                    break;
                case EDrawStyle.Luminance:
                    DrawContent_Luminance();
                    break;
                case EDrawStyle.Red:
                    DrawContent_Red();
                    break;
                case EDrawStyle.Green:
                    DrawContent_Green();
                    break;
                case EDrawStyle.Blue:
                    DrawContent_Blue();
                    break;
            };
        }

        // 内部函数,绘制标记
        private void DrawMarker(Point pos, bool conditional)
        {
            // 限定范围
            if (mMarkerPos.X < 0) mMarkerPos.X = 0;
            if (mMarkerPos.Y < 0) mMarkerPos.Y = 0;
            if (mMarkerPos.X > Width - 4) mMarkerPos.X = Width - 4;
            if (mMarkerPos.Y > Height - 4) mMarkerPos.Y = Height - 4;

            if (mMarkerPos == pos && !conditional)
                return;

            // 
            ClearMarker();

            // 更新位置
            mMarkerPos = pos;

            // 绘制Marker
            Graphics g = CreateGraphics();
            DrawMarkerOnCanvas(g, pos);
        }

        // 内部函数,绘制标记
        private void DrawMarkerOnCanvas(Graphics g, Point pos)
        {
            // The selected color determines the color of the marker drown over
            Pen pencil;
            ColorSpaceConverter.HSL hsl = GetColor(pos);
            if (hsl.L < 200 / 255.0)
            {
                pencil = new Pen(Color.White);
            }
            else if (hsl.H < 26 / 360.0 || hsl.H > 200 / 360.0)
            {
                if (hsl.S > 70 / 255.0)
                    pencil = new Pen(Color.White);
                else
                    pencil = new Pen(Color.Black);
            }
            else
            {
                pencil = new Pen(Color.Black);
            }

            // Draw the marker : 11 x 11 circle
            g.DrawEllipse(pencil, pos.X - 3, pos.Y - 3, 10, 10);

            // Force the border to be redrawn, just in case the marker has been drawn over it
            DrawBorder();
        }

        // 内部函数
        private void DrawContent_Hue()
        {
            //      S=0, S=1, S=2, S=3, S=4 ... S=100
            // L=100
            // L=99
            // L=98         DrawStyleHue
            // L=97
            // ...
            // L=0
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            ColorSpaceConverter.HSL startHSL = new ColorSpaceConverter.HSL();
            ColorSpaceConverter.HSL endHSL = new ColorSpaceConverter.HSL();
            startHSL.H = mHSL.H;
            startHSL.S = 0.0;
            endHSL.H = mHSL.H;
            endHSL.S = 1.0;

            // For each Horizontal line in the control, 
            // caculate luminance at this line (Hue and Saturation are constant)
            for (int i = 0; i < Height - 4; ++i)
            {
                startHSL.L = 1.0 - i / (Height - 4.0);
                endHSL.L = startHSL.L;

                LinearGradientBrush brush = new LinearGradientBrush(
                    new Rectangle(2, 2, Width - 4, 1),
                    ColorSpaceConverter.HSL2RGB(startHSL),
                    ColorSpaceConverter.HSL2RGB(endHSL),
                    0, false);

                g.FillRectangle(brush, new Rectangle(2, i + 2, Width - 4, 1));
            }

            //
            DrawMarkerOnCanvas(g, mMarkerPos);

            //
            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Saturation()
        {
            //      H=0, H=1, H=2, H=3, H=4 ... H=100
            // L=100
            // L=99
            // L=98         DrawStyleSaturation
            // L=97
            // ...
            // L=0
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            ColorSpaceConverter.HSL startHSL = new ColorSpaceConverter.HSL();
            ColorSpaceConverter.HSL endHSL = new ColorSpaceConverter.HSL();
            startHSL.S = mHSL.S;
            startHSL.L = 1.0;
            endHSL.S = mHSL.S;
            endHSL.L = 0.0;

            for (int i = 0; i < Width - 4; ++i)
            {
                startHSL.H = i / (Width - 4.0);
                endHSL.H = startHSL.H;

                LinearGradientBrush brush = new LinearGradientBrush(
                    new Rectangle(2, 2, 1, Height - 4),
                    ColorSpaceConverter.HSL2RGB(startHSL),
                    ColorSpaceConverter.HSL2RGB(endHSL),
                    90, false);

                g.FillRectangle(brush, new Rectangle(i + 2, 2, 1, Height - 4));
            }

            DrawMarkerOnCanvas(g, mMarkerPos);

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Luminance()
        {
            //      H=0, H=1, H=2, H=3, H=4 ... H=100
            // S=100
            // S=99
            // S=98         DrawStyleLuminance
            // S=97
            // ...
            // S=0
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            ColorSpaceConverter.HSL startHSL = new ColorSpaceConverter.HSL();
            ColorSpaceConverter.HSL endHSL = new ColorSpaceConverter.HSL();
            startHSL.S = 1.0;
            startHSL.L = mHSL.L;
            endHSL.S = 0.0;
            endHSL.L = mHSL.L;

            for (int i = 0; i < Width - 4; ++i)
            {
                startHSL.H = i / (Width - 4.0);
                endHSL.H = startHSL.H;

                LinearGradientBrush brush = new LinearGradientBrush(
                    new Rectangle(2, 2, 1, Height - 4),
                    ColorSpaceConverter.HSL2RGB(startHSL),
                    ColorSpaceConverter.HSL2RGB(endHSL),
                    90, false);

                g.FillRectangle(brush, new Rectangle(i + 2, 2, 1, Height - 4));
            }

            DrawMarkerOnCanvas(g, mMarkerPos);

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Red()
        {
            //      B=0, B=1, B=2, B=3, B=4 ... B=100
            // G=100
            // G=99
            // G=98         DrawStyleRed
            // G=97
            // ...
            // G=0
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            int red = mRGB.R;
            for (int i = 0; i < Height - 4; ++i)
            {
                int green = Round(255 - (255 * i / (Height - 4.0)));


                LinearGradientBrush brush = new LinearGradientBrush(
                    new Rectangle(2, 2, Width - 4, 1),
                    Color.FromArgb(red, green, 0),
                    Color.FromArgb(red, green, 255),
                    0, false);

                g.FillRectangle(brush, new Rectangle(2, i + 2, Width - 4, 1));
            }

            DrawMarkerOnCanvas(g, mMarkerPos);

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Green()
        {
            //      B=0, B=1, B=2, B=3, B=4 ... B=100
            // R=100
            // R=99
            // R=98         DrawStyleGreen
            // R=97
            // ...
            // R=0
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            int green = mRGB.G;
            for (int i = 0; i < Height - 4; ++i)
            {
                int red = Round(255 - (255 * i / (Height - 4.0)));

                LinearGradientBrush brush = new LinearGradientBrush(
                    new Rectangle(2, 2, Width - 4, 1),
                    Color.FromArgb(red, green, 0),
                    Color.FromArgb(red, green, 255),
                    0, false);

                g.FillRectangle(brush, new Rectangle(2, i + 2, Width - 4, 1));
            }

            DrawMarkerOnCanvas(g, mMarkerPos);

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Blue()
        {
            //      R=0, R=1, R=2, R=3, R=4 ... R=100
            // G=100
            // G=99
            // G=98         DrawStyleGreen
            // G=97
            // ...
            // G=0
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            int blue = mRGB.B;
            for (int i = 0; i < Height - 4; ++i)
            {
                int green = Round(255 - (255 * i / (Height - 4.0)));

                LinearGradientBrush brush = new LinearGradientBrush(
                    new Rectangle(2, 2, Width - 4, 1),
                    Color.FromArgb(0, green, blue),
                    Color.FromArgb(255, green, blue),
                    0, false);

                g.FillRectangle(brush, new Rectangle(2, i + 2, Width - 4, 1));
            }

            DrawMarkerOnCanvas(g, mMarkerPos);

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void ResetColor()
        {
            int red, green, blue;
            switch (mDrawStyle)
            {
                case EDrawStyle.Hue:
                    mHSL.S = mMarkerPos.X / (Width - 4.0);
                    mHSL.L = 1.0 - mMarkerPos.Y / (Height - 4.0);
                    mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
                    break;
                case EDrawStyle.Saturation:
                    mHSL.H = mMarkerPos.X / (Width - 4.0);
                    mHSL.L = 1.0 - mMarkerPos.Y / (Height - 4.0);
                    mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
                    break;
                case EDrawStyle.Luminance:
                    mHSL.H = mMarkerPos.X / (Width - 4.0);
                    mHSL.S = 1.0 - mMarkerPos.Y / (Height - 4.0);
                    mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
                    break;
                case EDrawStyle.Red:
                    blue = Round(255 * mMarkerPos.X / (Width - 4.0));
                    green = Round(255 * (1.0 - mMarkerPos.Y / (Height - 4.0)));
                    mRGB = Color.FromArgb(mRGB.R, green, blue);
                    mHSL = ColorSpaceConverter.RGB2HSL(mRGB);
                    break;
                case EDrawStyle.Green:
                    blue = Round(255 * mMarkerPos.X / (Width - 4.0));
                    red = Round(255 * (1.0 - mMarkerPos.Y / (Height - 4.0)));
                    mRGB = Color.FromArgb(red, mRGB.G, blue);
                    mHSL = ColorSpaceConverter.RGB2HSL(mRGB);
                    break;
                case EDrawStyle.Blue:
                    red = Round(255 * mMarkerPos.X / (Width - 4.0));
                    green = Round(255 * (1.0 - mMarkerPos.Y / (Height - 4.0)));
                    mRGB = Color.FromArgb(red, green, mRGB.B);
                    mHSL = ColorSpaceConverter.RGB2HSL(mRGB);
                    break;
            };
        }

        private void ResetMarker(bool redraw)
        {
            switch (mDrawStyle)
            {
                case EDrawStyle.Hue:
                    mMarkerPos.X = Round((Width - 4) * mHSL.S);
                    mMarkerPos.Y = Round((Height - 4) * (1 - mHSL.L));
                    break;
                case EDrawStyle.Saturation:
                    mMarkerPos.X = Round((Width - 4) * mHSL.H);
                    mMarkerPos.Y = Round((Height - 4) * (1 - mHSL.L));
                    break;
                case EDrawStyle.Luminance:
                    mMarkerPos.X = Round((Width - 4) * mHSL.H);
                    mMarkerPos.Y = Round((Height - 4) * (1 - mHSL.S));
                    break;
                case EDrawStyle.Red:
                    mMarkerPos.X = Round((Width - 4) * (mRGB.B / 255.0));
                    mMarkerPos.Y = Round((Height - 4) * (1 - mRGB.G / 255.0));
                    break;
                case EDrawStyle.Green:
                    mMarkerPos.X = Round((Width - 4) * (mRGB.B / 255.0));
                    mMarkerPos.Y = Round((Height - 4) * (1 - mRGB.R / 255.0));
                    break;
                case EDrawStyle.Blue:
                    mMarkerPos.X = Round((Width - 4) * (mRGB.R / 255.0));
                    mMarkerPos.Y = Round((Height - 4) * (1 - mRGB.G / 255.0));
                    break;
            };
        }

        // 内部函数,重新绘制当前位置的Marker为背景色
        private void ClearMarker()
        {
            // Determine the area that needs to be redrawn, find the markers corners
            int startX = mMarkerPos.X - 5;
            int startY = mMarkerPos.Y - 5;
            int endX = mMarkerPos.X + 5;
            int endY = mMarkerPos.Y + 5;

            // Adjust the area if part of it hangs outside the content area
            if (startX < 0) startX = 0;
            if (startY < 0) startY = 0;
            if (endX > Width - 4) endX = Width - 4;
            if (endY > Height - 4) endY = Height - 4;

            // Redraw the content based on the current draw style
            ColorSpaceConverter.HSL startHSL = new ColorSpaceConverter.HSL();
            ColorSpaceConverter.HSL endHSL = new ColorSpaceConverter.HSL();
            Graphics g = this.CreateGraphics();

            int red, green, blue;
            switch (mDrawStyle)
            {
                //      S=0, S=1, S=2, S=3, S=4 ... S=100
                // L=100
                // L=99
                // L=98         DrawStyleHue
                // L=97
                // ...
                // L=0
                case EDrawStyle.Hue :
                    startHSL.H = mHSL.H;                        ///< Hue is constant
                    endHSL.H = mHSL.H;                          ///< Hue is constant
                    startHSL.S = startX / (Width - 4.0);        ///< Because we are drawing horizontal lines,
                    endHSL.S = endX / (Width - 4.0);            ///< S not change from line to line

                    for (int i = startY; i <= endY; ++i)        ///< for each horizontal line
                    {
                        startHSL.L = 1 - i / (Height - 4.0);    ///< Hue will change for each horizontal
                        endHSL.L = startHSL.L;                  ///< line drawn

                        LinearGradientBrush brush = new LinearGradientBrush(
                            new Rectangle(startX + 1, i + 2, endX - startX + 1, 1),
                            ColorSpaceConverter.HSL2RGB(startHSL),
                            ColorSpaceConverter.HSL2RGB(endHSL),
                            0, false);

                        g.FillRectangle(brush, new Rectangle(startX + 2, i + 2, endX - startX + 1, 1));
                    }

                    break;

                //      H=0, H=1, H=2, H=3, H=4 ... H=360
                // L=100
                // L=99
                // L=98         DrawStyleSaturation
                // L=97
                // ...
                // L=0
                case EDrawStyle.Saturation:
                    startHSL.S = mHSL.S;                        ///< Saturation is constant
                    endHSL.S = mHSL.S;                          ///< Saturation is constant
                    startHSL.L = 1 - startY / (Height - 4.0);   ///< Because we are drawing vertical lines,
                    endHSL.L = 1 - endY / (Height - 4.0);       ///< L not change from line to line

                    for (int i = startX; i <= endX; ++i)        ///< for each vertical line
                    {
                        startHSL.H = i / (Width - 4.0);         ///< Hue will change for each vertical
                        endHSL.H = startHSL.H;                  ///< line drawn

                        LinearGradientBrush brush = new LinearGradientBrush(
                            new Rectangle(i + 2, startY + 1, 1, endY - startY + 2),
                            ColorSpaceConverter.HSL2RGB(startHSL),
                            ColorSpaceConverter.HSL2RGB(endHSL),
                            90, false);

                        g.FillRectangle(brush, new Rectangle(i + 2, startY + 2, 1, endY - startY + 1));
                    }

                    break;

                //      H=0, H=1, H=2, H=3, H=4 ... H=100
                // S=100
                // S=99
                // S=98         DrawStyleLuminance
                // S=97
                // ...
                // S=0
                case EDrawStyle.Luminance:
                    startHSL.L = mHSL.L;                        ///< Luminance is constant
                    endHSL.L = mHSL.L;                          ///< Luminance is constant
                    startHSL.S = 1 - startY / (Height - 4.0);   ///< Because we are drawing vertical lines 
                    endHSL.S = 1 - endY / (Height - 4.0);       ///< S not change from line to line

                    for (int i = startX; i <= endX; ++i)        ///< for each vertical line
                    {
                        startHSL.H = i / (Width - 4.0);         ///< Luminance will change for each vertical
                        endHSL.H = startHSL.H;                  ///< line drawn

                        LinearGradientBrush brush = new LinearGradientBrush(
                            new Rectangle(i + 2, startY+1, 1, endY-startY+2),
                            ColorSpaceConverter.HSL2RGB(startHSL),
                            ColorSpaceConverter.HSL2RGB(endHSL),
                            90, false);

                        g.FillRectangle(brush, new Rectangle(i + 2, startY + 2, 1, endY - startY + 1));
                    }

                    break;

                //      B=0, B=1, B=2, B=3, B=4 ... B=100
                // G=100
                // G=99
                // G=98         DrawStyleRed
                // G=97
                // ...
                // G=0
                case EDrawStyle.Red:
                    red = mRGB.R;                                           ///< Red is constant
                    int startB = Round(255 * startX / (Width - 4.0));
                    int endB = Round(255 * endX / (Width - 4.0));

                    for (int i = startY; i <= endY; ++i)
                    {
                        green = Round(255 - (255 * i / (Height - 4.0)));

                        LinearGradientBrush brush = new LinearGradientBrush(
                            new Rectangle(startX + 1, i + 2, endX-startX + 1, 1),
                            Color.FromArgb(red, green, startB),
                            Color.FromArgb(red, green, endB),
                            0, false);

                        g.FillRectangle(brush, new Rectangle(startX + 2, i + 2, endX - startX + 1, 1));
                    }

                    break;

                //      B=0, B=1, B=2, B=3, B=4 ... B=100
                // R=100
                // R=99
                // R=98         DrawStyleGreen
                // R=97
                // ...
                // R=0
                case EDrawStyle.Green:
                    green = mRGB.G;
                    int startB2 = Round(255 * startX / (Width - 4.0));
                    int endB2 = Round(255 * endX / (Width - 4.0));

                    for (int i = startY; i <= endY; ++i)
                    {
                        red = Round(255 - (255 * i / (Height - 4.0)));

                        LinearGradientBrush brush = new LinearGradientBrush(
                            new Rectangle(startX + 1, i + 2, endX - startX + 1, 1),
                            Color.FromArgb(red, green, startB2),
                            Color.FromArgb(red, green, endB2),
                            0, false);

                        g.FillRectangle(brush, new Rectangle(startX + 2, i + 2, endX - startX + 1, 1));
                    }

                    break;

                //      R=0, R=1, R=2, R=3, R=4 ... R=100
                // G=100
                // G=99
                // G=98         DrawStyleGreen
                // G=97
                // ...
                // G=0
                case EDrawStyle.Blue:
                    blue = mRGB.B;
                    int startR = Round(255 * startX / (Width - 4.0));
                    int endR = Round(255 * endX / (Width - 4.0));

                    for (int i = startY; i <= endY; ++i)
                    {
                        green = Round(255 - (255 * i / (Height - 4.0)));

                        LinearGradientBrush brush = new LinearGradientBrush(
                            new Rectangle(startX + 1, i + 2, endX - startX + 1, 1),
                            Color.FromArgb(startR, green, blue),
                            Color.FromArgb(endR, green, blue),
                            0, false);

                        g.FillRectangle(brush, new Rectangle(startX + 2, i + 2, endX - startX + 1, 1));
                    }

                    break;
            }
        }

        // 内部函数, 获取当前位置颜色
        private ColorSpaceConverter.HSL GetColor(Point pos)
        {
            int red, green, blue;
            ColorSpaceConverter.HSL hsl = new ColorSpaceConverter.HSL();
            switch (mDrawStyle)
            {
                case EDrawStyle.Hue:
                    hsl.H = mHSL.H;
                    hsl.S = pos.X / (Width - 4.0);
                    hsl.L = 1.0 - pos.Y / (Height - 4.0);
                    break;
                case EDrawStyle.Saturation:
                    hsl.S = mHSL.S;
                    hsl.H = pos.X / (Width - 4.0);
                    hsl.L = 1.0 - pos.Y / (Height - 4.0);
                    break;
                case EDrawStyle.Luminance:
                    hsl.L = mHSL.L;
                    hsl.H = pos.X / (Width - 4.0);
                    hsl.S = 1.0 - pos.Y / (Height - 4.0);
                    break;
                case EDrawStyle.Red:
                    blue = Round(255 * mMarkerPos.X / (Width - 4.0));
                    green = Round(255 * (1.0 - mMarkerPos.Y / (Height - 4.0)));
                    hsl = ColorSpaceConverter.RGB2HSL(Color.FromArgb(mRGB.R, green, blue));
                    break;
                case EDrawStyle.Green:
                    blue = Round(255 * mMarkerPos.X / (Width - 4.0));
                    red = Round(255 * (1.0 - mMarkerPos.Y / (Height - 4.0)));
                    hsl = ColorSpaceConverter.RGB2HSL(Color.FromArgb(red, mRGB.G, blue));
                    break;
                case EDrawStyle.Blue:
                    red = Round(255 * mMarkerPos.X / (Width - 4.0));
                    green = Round(255 * (1.0 - mMarkerPos.Y / (Height - 4.0)));
                    hsl = ColorSpaceConverter.RGB2HSL(Color.FromArgb(red, green, mRGB.B));
                    break;
            };

            return hsl;
        }

        // 四舍五入取整
        private static int Round(double val)
        {
            int ret = (int)val;
            int tmp = (int)(val * 100);
            if ((tmp % 100) >= 50)
                ret += 1;

            return ret;
        }
    }
}
