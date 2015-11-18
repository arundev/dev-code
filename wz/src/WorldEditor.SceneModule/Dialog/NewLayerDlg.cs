using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditorFramework;

namespace WorldEditor.SceneModule.Dialog
{
    public partial class NewLayerDlg : Form
    {
        private OpenFileDialog mFileDlg;    ///< 临时

        public NewLayerDlg()
        {
            InitializeComponent();

            // 资源对话框
            mFileDlg = new OpenFileDialog();
            mFileDlg.InitialDirectory = "E:\\project_game\\edata\\world0\\texture\\terrain";
        }

        public string LayerName
        {
            get { return textBoxLayerName.Text; }
            set { textBoxLayerName.Text = value; }
        }

        private string mBaseMap = "";
        public string BaseMap
        {
            get { return mBaseMap; }
            set
            {
                mBaseMap = value;
                textBoxLayerDiffuseMap.Text = value;

                IntPtr hBitmap = MTextureHelper.LoadImageHBMP(value);
                if (hBitmap == IntPtr.Zero)
                {
                    // ILoggingService
                    // String strMsg = String.Format("读取图片{0:s}失败!", value);
                    pictureBoxLayerDiffuseMap.Image = null;
                }
                else
                {
                    pictureBoxLayerDiffuseMap.Image = Bitmap.FromHbitmap(hBitmap);
                    MTextureHelper.ReleaseImageHBMP(hBitmap);
                }
            }
        }

        private string mNormalMap = "";
        public string NormalMap
        {
            get { return mNormalMap; }
            set
            {
                mNormalMap = value;
                textBoxLayerNormalMap.Text = value;

                IntPtr hBitmap = MTextureHelper.LoadImageHBMP(value);
                if (hBitmap == IntPtr.Zero)
                {
                    // ILoggingService
                    // String strMsg = String.Format("读取图片{0:s}失败!", value);
                    pictureBoxLayerNormalMap.Image = null;
                }
                else
                {
                    pictureBoxLayerNormalMap.Image = Bitmap.FromHbitmap(hBitmap);
                    MTextureHelper.ReleaseImageHBMP(hBitmap);
                }
            }
        }

        public float UVRepeat
        {
            get { return (float)Convert.ToDouble(trackerBarWidgetLayerUVRepeat.Value); }
            set { trackerBarWidgetLayerUVRepeat.Value = Convert.ToDecimal(value); }
        }

        public float MipmapBias
        {
            get { return (float)Convert.ToDouble(trackerBarWidgetLayerMipmapBias.Value); }
            set { trackerBarWidgetLayerMipmapBias.Value = Convert.ToDecimal(value); }
        }

        public bool Alpha
        {
            get { return checkBoxLayerAlpha.Checked; }
            set { checkBoxLayerAlpha.Checked = value; }
        }

        private void pictureBoxLayerDiffuseMap_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == mFileDlg.ShowDialog())
            {
                BaseMap = mFileDlg.FileName;

                // 计算法线贴图名称(检校不完整，可能出错)
                String strNormalMap = mFileDlg.FileName;
                strNormalMap = strNormalMap.Replace("_COLOR", "");
                strNormalMap = strNormalMap.Replace("_color", "");
                strNormalMap = strNormalMap.Replace(".dds", "_nrm.dds");
                strNormalMap = strNormalMap.Replace(".DDS", "_nrm.dds");
                if (File.Exists(strNormalMap))
                    NormalMap = strNormalMap;
            }
        }

        private void pictureBoxLayerNormalMap_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == mFileDlg.ShowDialog())
            {
                mNormalMap = mFileDlg.FileName;
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (LayerName == "")
            {
                MessageBox.Show("图层名不能为空!");
                this.DialogResult = DialogResult.Cancel;;
            }
            else if (BaseMap == "" || NormalMap == "")
            {
                MessageBox.Show("贴图不能为空!");
                this.DialogResult = DialogResult.Cancel;
            }
            else
            {
                this.DialogResult = DialogResult.OK;
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }
    }
}
