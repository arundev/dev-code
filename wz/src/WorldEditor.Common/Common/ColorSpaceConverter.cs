using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace WorldEditor.Common.Common
{
    // 
    // 参考资料:
    //  http://en.wikipedia.org/wiki/Color_space
    //  http://en.wikipedia.org/wiki/RGB_color_model
    //  http://en.wikipedia.org/wiki/CMYK_color_model
    //  http://en.wikipedia.org/wiki/HSL_and_HSV
    //  http://zh.wikipedia.org/wiki/RGB_色彩空间
    //  http://zh.wikipedia.org/wiki/印刷四分色模式
    //  http://zh.wikipedia.org/wiki/HSL和HSV色彩空间

    public class ColorSpaceConverter
    {
        // HSL颜色空间[0,1]
        //  色相（hue,H）           是色彩的基本属性，就是平常所说的颜色名称，如红色、黄色等。
        //  饱和度（saturation,S）  是指色彩的纯度，越高色彩越纯，低则逐渐变灰，取0-100%的数值。
        //  亮度（lightness,L）     取0-100%。
        public class HSL
        {
            private double mH;
            public double H
            {
                get { return mH;}
                set { mH = Saturate(value); }
            }

            private double mS;
            public double S
            {
                get { return mS; }
                set { mS = Saturate(value); }
            }

            private double mL;
            public double L
            {
                get { return mL; }
                set { mL = Saturate(value); }
            }

            public HSL()
            {
                mH = mS = mL = 0;
            }


            public HSL(double h, double s, double l)
            {
                H = h; S = s; L = l;
            }

            private double Saturate(double dValue)
            {
                return dValue > 1 ? 1 : dValue < 0 ? 0 : dValue;
            }
        }

        // CMYK颜色空间[0,1]
        //  C：Cyan ＝ 青色，在英文的翻译里是名词‘青绿色’。
        //  M：Magenta ＝ 品红色，又称为‘洋红色’。
        //  Y：Yellow ＝ 黄色。
        //  K：blacK ＝ 黑色，为了避免与RGB的Blue蓝色混淆而改称K。
        public class CMYK
        {
            private double mC;
            public double C
            {
                get { return mC; }
                set { mC = Saturate(value); }
            }

            private double mM;
            public double M
            {
                get { return mM; }
                set { mM = Saturate(value); }
            }

            private double mY;
            public double Y
            {
                get { return mY; }
                set { mY = Saturate(value); }
            }

            private double mK;
            public double K
            {
                get { return mK; }
                set { mK = Saturate(value); }
            }

            public CMYK()
            {
                mC = mM = mY = mK = 0;
            }

            public CMYK(double c, double m, double y, double k)
            {
                C = c; M = m; Y = y; K = k;
            }

            private double Saturate(double dValue)
            {
                return dValue > 1 ? 1 : dValue < 0 ? 0 : dValue;
            }
        }

        // (貌似是仿照Photoshop,和wiki的不大一样)
        public static HSL RGB2HSL(Color rgb)
        {
            HSL hsl = new HSL();

            // 求出RGB的最大值和最小值
            int max, min, diff, sum;
            max = Math.Max(rgb.R, rgb.G);
            max = Math.Max(max, rgb.B);
            min = Math.Min(rgb.R, rgb.G);
            min = Math.Min(min, rgb.B);

            diff = max - min;
            sum  = max + min;

            // Luminance/Brightness
            //  -Adobe photoshop uses the logic that the site VBspeed regards(regarded)
            //   as too primitive = supperior decides the level of brightness
            hsl.L = max / 255.0;

            // Saturation
            //  -Adobe photoshop uses this logic
            if (max == 0)  hsl.S = 0;
            else hsl.S = diff / (double)max;

            // Hue
            //  -R is situated at the degree of 360 eller noll degree
            //  -G vid 120 degree
            //  -B vid 240 degree
            double q;
            if (diff == 0) q = 0;
            else q = 60.0 / diff;

            if (max == rgb.R)
            {
                if (rgb.G < rgb.B)
                    hsl.H = (360 + q * (rgb.G - rgb.B)) / 360.0;
                else
                    hsl.H = q * (rgb.G - rgb.B) / 360.0;
            }
            else if (max == rgb.G)
            {
                hsl.H = (120 + q * (rgb.B - rgb.R)) / 360.0;
            }
            else if (max == rgb.B)
            {
                hsl.H = (240 + q * (rgb.R - rgb.G)) / 360.0;
            }
            else
            {
                hsl.H = 0.0;
            }

            return hsl;
        }

        // Adapted from the algorithm in Foley and Van-Dam
        // (Computer Graphics: Principles and Practice)
        public static Color HSL2RGB(HSL hsl)
        {
            int max, mid, min;
            double q;

            max = Round(hsl.L * 255);
            min = Round((1.0 - hsl.S) * (hsl.L / 1.0) * 255);
            q = (max - min) / 255.0;

            if (hsl.H >= 0 && hsl.H <= 1 / 6.0)
            {
                mid = Round(((hsl.H - 0) * q) * 1530 + min);
                return Color.FromArgb(max, mid, min);
            }
            else if (hsl.H <= 1 / 3.0)
            {
                mid = Round(-((hsl.H - 1 / 6.0) * q) * 1530 + max);
                return Color.FromArgb(mid, max, min);
            }
            else if (hsl.H <= 1 / 2.0)
            {
                mid = Round(((hsl.H - 1 / 3.0) * q) * 1530 + min);
                return Color.FromArgb(min, max, mid);
            }
            else if (hsl.H <= 2 / 3.0)
            {
                mid = Round(-((hsl.H - 1 / 2.0) * q) * 1530 + max);
                return Color.FromArgb(min, mid, max);
            }
            else if (hsl.H <= 5 / 6.0)
            {
                mid = Round(((hsl.H - 2 / 3.0) * q) * 1530 + min);
                return Color.FromArgb(mid, min, max);
            }
            else if (hsl.H <= 1.0)
            {
                mid = Round(-((hsl.H - 5 / 6.0) * q) * 1530 + max);
                return Color.FromArgb(max, min, mid);
            }
            else
            {
                return Color.Black;
            }
        }

        public static CMYK RGB2CMYK(Color rgb)
        {
            // 从RGB转到CMY
            double c = 1.0 - rgb.R / 255.0;
            double m = 1.0 - rgb.G / 255.0;
            double y = 1.0 - rgb.B / 255.0;

            // 计算K分量
            double k = 1.0;
            k = c < k ? c : k;
            k = m < k ? m : k;
            k = y < k ? y : k;

            if (k == 1.0)
            {
                return new CMYK(0, 0, 0, 1.0);
            }
            else
            {
                CMYK cmyk = new CMYK();
                cmyk.C = (c - k) / (1 - k);
                cmyk.M = (m - k) / (1 - k);
                cmyk.Y = (y - k) / (1 - k);
                cmyk.K = k;

                return cmyk;
            }
        }

        public static Color CMYK2RGB(CMYK cmyk)
        {
            // 从CMYK转换到CMY
            double k = cmyk.K;
            double c = cmyk.C * (1 - k) + k;
            double m = cmyk.M * (1 - k) + k;
            double y = cmyk.Y * (1 - k) + k;

            // CMY转换到RGB
            int red   = Round((1 - c) * 255);
            int green = Round((1 - m) * 255);
            int blue  = Round((1 - y) * 255);

            return Color.FromArgb(red, green, blue);
        }

        public static String RGB2HexString(Color rgb)
        {
            String red = Convert.ToString(rgb.R, 16);
            if (red.Length < 2) red = "0" + red;

            String green = Convert.ToString(rgb.G, 16);
            if (green.Length < 2) green = "0" + green;

            String blue = Convert.ToString(rgb.B, 16);
            if (blue.Length < 2) blue = "0" + blue;

            return red.ToUpper() + green.ToUpper() + blue.ToUpper();
        }

        public static Color HexString2RGB(String hexString)
        {
            if (hexString.Length != 6)
                return Color.Black;

            int red = int.Parse(hexString.Substring(0, 2), System.Globalization.NumberStyles.HexNumber);
            int green = int.Parse(hexString.Substring(2, 2), System.Globalization.NumberStyles.HexNumber);
            int blue = int.Parse(hexString.Substring(4, 2), System.Globalization.NumberStyles.HexNumber);
        
            return Color.FromArgb(red, green, blue);
        }

        // 四舍五入取整
        private static int Round(double dValue)
        {
            int iRet = (int)dValue;
            int iTemp = (int)(dValue * 100);
            if ((iTemp % 100) >= 50)
                iRet += 1;
            return iRet;
        }
    }
}
