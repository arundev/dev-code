using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WorldEditor.SceneModule.Window
{
    public partial class PropertyForm : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        //[DockPosition(ShowOnLoad = true, Position = DefaultDock.DockRight)]
        public PropertyForm()
        {
            InitializeComponent();

            // 
        }

        #region 控件消息响应
        private void propertyGridProp_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {

        }
        #endregion
    }
}
