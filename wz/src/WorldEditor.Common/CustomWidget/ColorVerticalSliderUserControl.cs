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
    public partial class ColorVerticalSliderUserControl : UserControl
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

                ResetSlider(true);
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

                ResetSlider(true);
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

                ResetSlider(true);
                DrawControl();
            }
        }

        // 当前标记显示
        private bool mDragging = false;
        private int mSliderPos;

        // 编辑事件
        public new event EventHandler Scroll;

        public ColorVerticalSliderUserControl()
        {
            InitializeComponent();

            mHSL = new ColorSpaceConverter.HSL(1.0, 1.0, 1.0);
            mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
            mDrawStyle = EDrawStyle.Hue;
            mSliderPos = 0;
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
        private void ColorVerticalSliderUserControl_Load(object sender, EventArgs e)
        {
            DrawControl();
        }

        private void ColorVerticalSliderUserControl_Paint(object sender, PaintEventArgs e)
        {
            DrawControl();
        }

        private void ColorVerticalSliderUserControl_Resize(object sender, EventArgs e)
        {
            DrawControl();
        }

        private void ColorVerticalSliderUserControl_MouseDown(object sender, MouseEventArgs e)
        {
            // Only respond to left mouse button events
            if (e.Button != MouseButtons.Left)
                return;

            // Begin dragging which notifies MouseMove function that it needs to update the marker
            mDragging = true;

            // Calculate slider position
            int y = e.Y - 4;
            if (y < 0)  y = 0;
            if (y > Height - 9) y = Height - 9;

            // If the slider hasn't moved, no need to redraw it
            if (y == mSliderPos)
                return;
            
            // Redraw the Slider and Reset the color
            DrawSlider(y, false);
            ResetColor();

            // Notify anyone who cares that the controls slider(color) has changed
            if (Scroll != null)
                Scroll(this, e);  
        }

        private void ColorVerticalSliderUserControl_MouseUp(object sender, MouseEventArgs e)
        {
            // Only respond to left mouse button events
            if (e.Button != MouseButtons.Left)
                return;

            // 
            mDragging = false;

            // Calculate slider position
            int y = e.Y - 4;
            if (y < 0) y = 0;
            if (y > Height - 9) y = Height - 9;

            // If the slider hasn't moved, no need to redraw it
            if (y == mSliderPos)
                return;

            // Redraw the Slider and Reset the color
            DrawSlider(y, false);
            ResetColor();

            // Notify anyone who cares that the controls slider(color) has changed
            if (Scroll != null)
                Scroll(this, e);  
        }

        private void ColorVerticalSliderUserControl_MouseMove(object sender, MouseEventArgs e)
        {
            // Only respond when the mouse is dragging the marker
            if (!mDragging)
                return;

            // Calculate slider position
            int y = e.Y - 4;
            if (y < 0) y = 0;
            if (y > Height - 9) y = Height - 9;

            // If the slider hasn't moved, no need to redraw it
            if (y == mSliderPos)
                return;

            // Redraw the Slider and Reset the color
            DrawSlider(y, false);
            ResetColor();

            // Notify anyone who cares that the controls slider(color) has changed
            if (Scroll != null)
                Scroll(this, e); 
        }

        // 内部函数,绘制面板
        private void DrawControl()
        {
            DrawBorder();
            DrawContent();
        }

        // Draw the border around the control, in this case the border around the content
        // area between the slider arrows
        // 内部函数,绘制边框
        private void DrawBorder()
        {
            Graphics g = this.CreateGraphics();

            // To make the control look like Adobe Photoshop's,the border around the control
            // will be gray line on the top and left side. a white line on the bottom and 
            // the right side, and a black rectangle(line) inside the gray/white rectangle
            Pen pencil;

            pencil = new Pen(Color.FromArgb(172, 168, 153));    // The same gray color used by Photoshop
            g.DrawLine(pencil, Width - 10, 2, 9, 2);    // Draw top line
            g.DrawLine(pencil, 9, 2, 9, Height - 4);    // Draw left hand line

            pencil = new Pen(Color.White);
            g.DrawLine(pencil, Width - 9, 2, Width - 9, Height - 3);    // Draw right hand line
            g.DrawLine(pencil, Width - 9, Height - 3, 9, Height - 3);   // Draw bottom line

            pencil = new Pen(Color.Black);
            g.DrawRectangle(pencil, 10, 3, Width - 20, Height - 7);     // Draw inner black rectangle
        }

        // 内部函数,绘制颜色
        private void DrawContent()
        {
            switch (mDrawStyle)
            {
                case EDrawStyle.Hue:
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
        private void DrawSlider(int position, bool conditional)
        {
            // 限定范围
            if (position < 0) mSliderPos = 0;
            if (position > Height - 9) position = Height - 9;

            // If the slider position hasn't changed since the last time was drawn,
            // we don't have to redraw
            if (mSliderPos == position && !conditional)
                return;

            // Remove old slider
            ClearSlider();

            // Update the slider position
            mSliderPos = position;

            // Draw slider
            Graphics g = CreateGraphics();
            Pen pencil = new Pen(Color.FromArgb(116, 114, 106));    // Same gray with Photoshop uses
            Brush brush = Brushes.White;

            Point[] arrow = new Point[7];
            arrow[0] = new Point(1, position);      // G   G
            arrow[1] = new Point(3, position);      // G    G
            arrow[2] = new Point(7, position + 4);  // G     G
            arrow[3] = new Point(3, position + 8);  // G      G
            arrow[4] = new Point(1, position + 8);  // G     G
            arrow[5] = new Point(0, position + 7);  // G    G
            arrow[6] = new Point(0, position + 1);  // G   G

            g.FillPolygon(brush, arrow);    // Fill left arrow with white
            g.DrawPolygon(pencil, arrow);   // Draw left arrow border with gray

            arrow[0] = new Point(Width - 2, position);      //    G   G
            arrow[1] = new Point(Width - 4, position);      //   G    G
            arrow[2] = new Point(Width - 8, position + 4);  //  G     G
            arrow[3] = new Point(Width - 4, position + 8);  // G      G
            arrow[4] = new Point(Width - 2, position + 8);  //  G     G
            arrow[5] = new Point(Width - 1, position + 7);  //   G    G
            arrow[6] = new Point(Width - 1, position + 1);  //    G   G

            g.FillPolygon(brush, arrow);    // Fill right arrow with white
            g.DrawPolygon(pencil, arrow);   // Draw right arrow border with gray
        }

        // Fills in the content of the control showing all values of Hue (from 0 to 360)
        private void DrawContent_Hue()
        {
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            ColorSpaceConverter.HSL hsl = new ColorSpaceConverter.HSL();
            hsl.S = 1.0;                                                ///< S and L will both be at 100% for this DrawStyle
            hsl.L = 1.0;                                                ///<

            for (int i = 0; i < Height - 8; ++i)                        ///< i represents the current line of pixels we want to draw horizontally
            {
                hsl.H = 1.0 - i / (Height - 8.0);
                Pen pencil = new Pen(ColorSpaceConverter.HSL2RGB(hsl)); ///< H(Hue) is based on the current vertical position
                g.DrawLine(pencil, 11, i + 4, Width - 11, i + 4);       ///< Get the color for this line
            }

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        // Fills in the content of the control showing all values of Saturation (from 0 to 100%)
        // for the given Hue and Luminance
        private void DrawContent_Saturation()
        {
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            ColorSpaceConverter.HSL hsl = new ColorSpaceConverter.HSL();
            hsl.H = mHSL.H;                                             ///< Use the H and L values of the current color
            hsl.L = mHSL.L;                                             ///<

            for (int i = 0; i < Height - 8; ++i)                        ///< i represents the current line of pixels we want to draw horizontally
            {
                hsl.S = 1 - i / (Height - 8.0);                         ///< S(Saturation) is based on the current vertical position
                Pen pencil = new Pen(ColorSpaceConverter.HSL2RGB(hsl)); ///< Get the color for this line 
                g.DrawLine(pencil, 11, i + 4, Width - 11, i + 4);
            }

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        // Fills in the content of the control showing all values of Luminance (from 0 to 100%)
        // for the given Huw and Saturation
        private void DrawContent_Luminance()
        {
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            ColorSpaceConverter.HSL hsl = new ColorSpaceConverter.HSL();
            hsl.H = mHSL.H;                                             ///< Use the H and S values of the current color
            hsl.S = mHSL.S;                                             ///<

            for (int i = 0; i < Height - 8; ++i)
            {
                hsl.L = 1 - i / (Height - 8.0);
                Pen pencil = new Pen(ColorSpaceConverter.HSL2RGB(hsl));
                g.DrawLine(pencil, 11, i + 4, Width - 11, i + 4);
            }

            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Red()
        {
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            // i represents the current line of pixels we want to draw horizontally
            for (int i = 0; i < Height - 8; ++i)
            {
                int red = 255 - Round(255 * i / (Height - 8.0));

                Pen pencil = new Pen(Color.FromArgb(red, mRGB.G, mRGB.B));
                g.DrawLine(pencil, 11, i + 4, Width - 11, i + 4);
            }

            //
            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Green()
        {
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            // i represents the current line of pixels we want to draw horizontally
            for (int i = 0; i < Height - 8; ++i)
            {
                int green = 255 - Round(255 * i / (Height - 8.0));

                Pen pencil = new Pen(Color.FromArgb(mRGB.R, green, mRGB.B));
                g.DrawLine(pencil, 11, i + 4, Width - 11, i + 4);
            }

            //
            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void DrawContent_Blue()
        {
            Bitmap memBitmap = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            Graphics g = Graphics.FromImage(memBitmap);

            // i represents the current line of pixels we want to draw horizontally
            for (int i = 0; i < Height - 8; ++i)
            {
                int blue = 255 - Round(255 * i / (Height - 8.0));

                Pen pencil = new Pen(Color.FromArgb(mRGB.R, mRGB.G, blue));
                g.DrawLine(pencil, 11, i + 4, Width - 11, i + 4);
            }

            //
            PaintEventArgs e = new PaintEventArgs(CreateGraphics(), ClientRectangle);
            e.Graphics.DrawImage(memBitmap, 0, 0);
            memBitmap.Dispose();
            g.Dispose();
        }

        private void ResetColor()
        {
            switch (mDrawStyle)
            {
                case EDrawStyle.Hue:
                    mHSL.H = 1 - mSliderPos / (Height - 9.0);
                    mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
                    break;
                case EDrawStyle.Saturation:
                    mHSL.S = 1 - mSliderPos / (Height - 9.0);
                    mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
                    break;
                case EDrawStyle.Luminance:
                    mHSL.L = 1 - mSliderPos / (Height - 9.0);
                    mRGB = ColorSpaceConverter.HSL2RGB(mHSL);
                    break;
                case EDrawStyle.Red:
                    mRGB = Color.FromArgb(255 - Round(255 * mSliderPos / (Height - 9.0)), mRGB.G, mRGB.B);
                    mHSL = ColorSpaceConverter.RGB2HSL(mRGB);
                    break;
                case EDrawStyle.Green:
                    mRGB = Color.FromArgb(mRGB.R, 255 - Round(255 * mSliderPos / (Height - 9.0)), mRGB.B);
                    mHSL = ColorSpaceConverter.RGB2HSL(mRGB);
                    break;
                case EDrawStyle.Blue:
                    mRGB = Color.FromArgb(mRGB.R, mRGB.G, 255 - Round(255 * mSliderPos / (Height - 9.0)));
                    mHSL = ColorSpaceConverter.RGB2HSL(mRGB);
                    break;
            };
        }

        private void ResetSlider(bool redraw)
        {
            switch (mDrawStyle)
            {
                case EDrawStyle.Hue:
                    mSliderPos = Height - 8 - Round((Height - 8) * mHSL.H);
                    break;
                case EDrawStyle.Saturation:
                    mSliderPos = Height - 8 - Round((Height - 8) * mHSL.S);
                    break;
                case EDrawStyle.Luminance:
                    mSliderPos = Height - 8 - Round((Height - 8) * mHSL.L);
                    break;
                case EDrawStyle.Red:
                    mSliderPos = Height - 8 - Round((Height - 8) * mRGB.R / 255.0);
                    break;
                case EDrawStyle.Green:
                    mSliderPos = Height - 8 - Round((Height - 8) * mRGB.G / 255.0);
                    break;
                case EDrawStyle.Blue:
                    mSliderPos = Height - 8 - Round((Height - 8) * mRGB.B / 255.0);
                    break;
            };

            if (redraw)
                DrawSlider(mSliderPos, true);
        }

        // 内部函数,重新绘制当前位置的Slider为背景色
        private void ClearSlider()
        {
            // Redraw the background over the slider area on both sides of the control
            Graphics g = this.CreateGraphics();
            Brush brush = System.Drawing.SystemBrushes.Control;
            g.FillRectangle(brush, 0, 0, 8, Height);            // Clear left hand slider
            g.FillRectangle(brush, Width - 8, 0, 8, Height);    // Clear right hand slider
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
