using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WorldEditor.Common.CustomWidget
{
    // 自定义的卷帘栏控件
    [ToolboxItem(true)]
    public partial class CollapseBoxWidget : System.Windows.Forms.Panel
    {
        // 是否折叠
        [Browsable(true)]
        private bool mExpand = true; 
        public bool Expand
        {
            get { return mExpand; }
            set { if (mExpand == value) return; mExpand = value; Rebuild();}
        }
    
        // 文字属性访问重载
        [Browsable(true)]
        public override string Text
        {
            get { return base.Text;}
            set { base.Text = value; Rebuild(); }
        }

        // DisplayRectangle属性访问重载,提供自动布局功能
        public override Rectangle DisplayRectangle
        {
            get
            {
                Rectangle kRectTitle = new Rectangle(new Point(5, 0), new Size(Width - 5 - 5, 20));
                Rectangle kRect = base.DisplayRectangle;
                kRect.Offset(0, kRectTitle.Height);
                kRect.Height -= kRectTitle.Height;

                return kRect;
            }
        }

        public CollapseBoxWidget()
        {
            InitializeComponent();
        }

        public CollapseBoxWidget(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
        }



        // 对控件进行重新布局,重新计算空间尺寸
        private void Rebuild()
        {
            // 禁用布局更新
            SuspendLayout();

            // 遍历所有下属控件
            //  1.根据展开状态设置可见性
            //  2.计算控件整体最大高度
            int iMaxY = 0;
            foreach (Control kControl in this.Controls)
            {
                kControl.Visible = mExpand;

                Point kPoint = kControl.Location;
                int iY = kPoint.Y + kControl.Height;
                if (iY > iMaxY) iMaxY = iY;
            }

            // 根据当前空间状态,调整控件最终尺寸
            //  1.展开状态,最终尺寸加10
            //  2.关闭状态,固定大小
            if (mExpand)
            {
                this.Height = iMaxY + 10 + Padding.Bottom;
            }
            else
            {
                Rectangle kRectTitle = new Rectangle(new Point(5, 0), new Size(Width - 5 - 5, 20));
                Height = kRectTitle.Height + Padding.Vertical;
            }

            // 恢复布局更新
            ResumeLayout(true);

            // 强制重绘
            Invalidate();
        }

        //*******************************************************************************
        // 虚函数继承
        //*******************************************************************************


        // 虚函数继承,响应鼠标单击事件,鼠标单击到标题栏时改变圈起状态
        protected override void OnClick(EventArgs e)
        {
            Rectangle kRectTitle = new Rectangle(new Point(5, 0), new Size(Width - 5 - 5, 20));
            Point kPoint = PointToClient(MousePosition);
            if (kRectTitle.Contains(kPoint))
            {
                Expand = !Expand;
            }

            base.OnClick(e);
        }

        // 虚函数继承,响应尺寸改变事件,尺寸改变时重绘控件
        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            Invalidate();
        }

        // 虚函数继承,响应滚动,滚动时重绘控件
        protected override void OnScroll(ScrollEventArgs se)
        {
            base.OnScroll(se);

            Invalidate();
        }

        // 虚函数继承,响应绘制事件,绘制控件
        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);

            // 提取图形对象
            Graphics kGraphics = e.Graphics;

            // 计算标题栏/边框/标题+号/标题文本包围盒
            Rectangle kRectTitle = new Rectangle(new Point(8, 4), new Size(Width - 8 - 8, 18));
            Rectangle kRectBorder = new Rectangle(new Point(0, kRectTitle.Y + kRectTitle.Height / 2 - 5), new Size(Width, Height - kRectTitle.Height / 2));
            if (!this.mExpand)    kRectBorder.Height = 10;
           
            Rectangle kRectCross = new Rectangle(new Point(kRectTitle.X + 5, 5), new Size(kRectTitle.X - 4 - 4, kRectTitle.Height - 4 - 4));
            Rectangle kRectText = new Rectangle(new Point(kRectCross.X + 5, 5), new Size(kRectTitle.Width - kRectCross.Width - 5 - 5, kRectTitle.Height));

            // 绘制边框
            kGraphics.ExcludeClip(kRectTitle);
            ControlPaint.DrawBorder3D(kGraphics, kRectBorder, Border3DStyle.Etched);

            // 绘制标题栏
            kGraphics.ResetClip();
            using (Brush kBrush = new SolidBrush(this.BackColor))
            {
                kGraphics.FillRectangle(kBrush, kRectTitle);
            }
            ButtonState kButtonState = mExpand ? ButtonState.Pushed : ButtonState.Normal;
            ControlPaint.DrawBorder(kGraphics, kRectTitle, Color.Black, ButtonBorderStyle.Solid);

            // 绘制标题按钮十字
            Point kPt1 = new Point();
            Point kPt2 = new Point();
            if (!mExpand)
            {
                kPt1.X = kRectCross.X + kRectCross.Width / 2;
                kPt1.Y = kRectCross.Y + 2;
                kPt2.X = kPt1.X;
                kPt2.Y = kPt1.Y + kRectCross.Height - 4;
                kGraphics.DrawLine(Pens.Black, kPt1, kPt2);
            }

            kPt1.X = kRectCross.X + 2;
            kPt1.Y = kRectCross.Y + kRectCross.Height / 2;
            kPt2.X = kPt1.X + kRectCross.Width - 4;
            kPt2.Y = kPt1.Y;
            kGraphics.DrawLine(Pens.Black, kPt1, kPt2);

            // 绘制标题按钮文字
            StringFormat kStringFormat = new StringFormat();
            kStringFormat.Alignment = StringAlignment.Center;
            kStringFormat.FormatFlags = StringFormatFlags.LineLimit;
            kStringFormat.LineAlignment = StringAlignment.Center;
            kGraphics.DrawString(this.Text, this.Font, Brushes.Black, kRectText);
        }
    }
}
