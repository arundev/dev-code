using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WorldEditor.Common.CustomWidget
{
    // TrackBar的一个组合控件
    public partial class TrackerBarWidget2 : UserControl
    {
        // 重载TrackBar,不显示内部子控件的焦点框
        private class InnerTrackBar : System.Windows.Forms.TrackBar
        {
            protected override bool ShowFocusCues
            {
                get
                {
                    return false;
                }
            }
        }

        // 控件数值改变事件
        [CategoryAttribute("SDProperty")]
        public event OnValueChangedHandler OnValueChanged;

        public TrackerBarWidget2()
        {
            InitializeComponent();
        }

        // 标题属性
        [CategoryAttribute("SDProperty")]
        public string LabelCaption
        {
            get { return this.label.Text; }
            set { this.label.Text = value; }
        }

        // 值
        private Decimal mValue;
        [CategoryAttribute("SDProperty")]
        public Decimal Value
        {
            get { return mValue; }
            set
            {
                mValue = value;
                numericUpDown.Value = value;
                trackBar.Invalidate();
                trackBar.Refresh();
            }
        }

        // 缩放比例
        private Decimal mInnerScale;
        [CategoryAttribute("SDProperty")]
        public Decimal InnerScale
        {
            get { return mInnerScale; }
            set
            {
                mInnerScale = value;
                trackBar.Minimum = 0;
                trackBar.Maximum = Convert.ToInt32((mMax - mMin) * mInnerScale);
                trackBar.Invalidate();
                trackBar.Refresh();
            }
        }

        // 最小值
        private Decimal mMin;
        [CategoryAttribute("SDProperty")]
        public Decimal MinValue
        {
            get { return mMin; }
            set
            {
                mMin = value;
                numericUpDown.Minimum = value;
                trackBar.Minimum = 0;
                trackBar.Maximum = Convert.ToInt32((mMax - mMin) * mInnerScale);
                trackBar.Invalidate();
                trackBar.Refresh();
            }
        }

        // 最大值
        private Decimal mMax;
        [CategoryAttribute("SDProperty")]
        public Decimal MaxValue
        {
            get { return mMax; }
            set
            {
                mMax = value;
                numericUpDown.Maximum = value;
                trackBar.Minimum = 0;
                trackBar.Maximum = Convert.ToInt32((mMax - mMin) * mInnerScale);
                trackBar.Invalidate();
                trackBar.Refresh();
            }
        }

        // 步距
        [CategoryAttribute("SDProperty")]
        public Decimal Increment
        {
            get { return numericUpDown.Increment; }
            set { numericUpDown.Increment = value; }
        }

        // 显示的小数点位数
        [CategoryAttribute("SDProperty")]
        public int DecimalPlaces
        {
            get { return numericUpDown.DecimalPlaces; }
            set { numericUpDown.DecimalPlaces = value; }
        }

        // 界面响应函数
        private bool bInner = false;
        private void trackBar_Scroll(object sender, EventArgs e)
        {
            if (bInner)
                return;

            // 更新numericUpDown
            bInner = true;
            numericUpDown.Value = Convert.ToDecimal(trackBar.Value / mInnerScale + mMin);
            bInner = false;

            // 更新数值
            mValue = numericUpDown.Value;

            // 发送改变消息
            if (OnValueChanged != null)
                OnValueChanged(sender, e);
        }

        private void numericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (bInner)
                return;

            // 更新TrackBar
            bInner = true;
            trackBar.Value = Convert.ToInt32((numericUpDown.Value - mMin) * mInnerScale);
            bInner = false;

            // 更新数值
            mValue = numericUpDown.Value;

            // 发送改变消息
            if (OnValueChanged != null)
                OnValueChanged(sender, e);
        }
    }
}
