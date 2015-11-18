namespace WorldEditor.SceneModule.Widget
{
    partial class SettingPanel
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBoxMappingChannel = new System.Windows.Forms.GroupBox();
            this.checkBoxDetailNormalMap = new System.Windows.Forms.CheckBox();
            this.checkBoxEnvironmentMap = new System.Windows.Forms.CheckBox();
            this.checkBoxAOMap = new System.Windows.Forms.CheckBox();
            this.checkBoxLightMap = new System.Windows.Forms.CheckBox();
            this.checkBoxSpecularMap = new System.Windows.Forms.CheckBox();
            this.checkBoxGlowMap = new System.Windows.Forms.CheckBox();
            this.checkBoxNormalMap = new System.Windows.Forms.CheckBox();
            this.checkBoxDiffuseMap = new System.Windows.Forms.CheckBox();
            this.groupBoxLightting = new System.Windows.Forms.GroupBox();
            this.checkBoxSpecular = new System.Windows.Forms.CheckBox();
            this.checkBoxDiffuse = new System.Windows.Forms.CheckBox();
            this.groupBoxLight = new System.Windows.Forms.GroupBox();
            this.checkBoxLocalLight = new System.Windows.Forms.CheckBox();
            this.checkBoxAssistLight = new System.Windows.Forms.CheckBox();
            this.checkBoxMainLight = new System.Windows.Forms.CheckBox();
            this.checkBoxAmbientLight = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBoxColorGrading = new System.Windows.Forms.CheckBox();
            this.checkBoxSoftParticle = new System.Windows.Forms.CheckBox();
            this.checkBoxHDR = new System.Windows.Forms.CheckBox();
            this.checkBoxGodRay = new System.Windows.Forms.CheckBox();
            this.checkBoxDof = new System.Windows.Forms.CheckBox();
            this.checkBoxMLAA = new System.Windows.Forms.CheckBox();
            this.checkBoxFog = new System.Windows.Forms.CheckBox();
            this.groupBoxEntityType = new System.Windows.Forms.GroupBox();
            this.groupBoxSelection = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBoxRenderLevel = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBoxMappingChannel.SuspendLayout();
            this.groupBoxLightting.SuspendLayout();
            this.groupBoxLight.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxMappingChannel
            // 
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxDetailNormalMap);
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxEnvironmentMap);
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxAOMap);
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxLightMap);
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxSpecularMap);
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxGlowMap);
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxNormalMap);
            this.groupBoxMappingChannel.Controls.Add(this.checkBoxDiffuseMap);
            this.groupBoxMappingChannel.Location = new System.Drawing.Point(3, 147);
            this.groupBoxMappingChannel.Name = "groupBoxMappingChannel";
            this.groupBoxMappingChannel.Size = new System.Drawing.Size(242, 111);
            this.groupBoxMappingChannel.TabIndex = 3;
            this.groupBoxMappingChannel.TabStop = false;
            this.groupBoxMappingChannel.Text = "贴图通道";
            // 
            // checkBoxDetailNormalMap
            // 
            this.checkBoxDetailNormalMap.AutoSize = true;
            this.checkBoxDetailNormalMap.Location = new System.Drawing.Point(97, 64);
            this.checkBoxDetailNormalMap.Name = "checkBoxDetailNormalMap";
            this.checkBoxDetailNormalMap.Size = new System.Drawing.Size(96, 16);
            this.checkBoxDetailNormalMap.TabIndex = 7;
            this.checkBoxDetailNormalMap.Text = "细节法线贴图";
            this.checkBoxDetailNormalMap.UseVisualStyleBackColor = true;
            this.checkBoxDetailNormalMap.CheckedChanged += new System.EventHandler(this.checkBoxDetailNormalMap_CheckedChanged);
            // 
            // checkBoxEnvironmentMap
            // 
            this.checkBoxEnvironmentMap.AutoSize = true;
            this.checkBoxEnvironmentMap.Location = new System.Drawing.Point(97, 86);
            this.checkBoxEnvironmentMap.Name = "checkBoxEnvironmentMap";
            this.checkBoxEnvironmentMap.Size = new System.Drawing.Size(72, 16);
            this.checkBoxEnvironmentMap.TabIndex = 6;
            this.checkBoxEnvironmentMap.Text = "环境贴图";
            this.checkBoxEnvironmentMap.UseVisualStyleBackColor = true;
            this.checkBoxEnvironmentMap.CheckedChanged += new System.EventHandler(this.checkBoxEnvironmentMap_CheckedChanged);
            // 
            // checkBoxAOMap
            // 
            this.checkBoxAOMap.AutoSize = true;
            this.checkBoxAOMap.Location = new System.Drawing.Point(7, 86);
            this.checkBoxAOMap.Name = "checkBoxAOMap";
            this.checkBoxAOMap.Size = new System.Drawing.Size(60, 16);
            this.checkBoxAOMap.TabIndex = 5;
            this.checkBoxAOMap.Text = "AO贴图";
            this.checkBoxAOMap.UseVisualStyleBackColor = true;
            this.checkBoxAOMap.CheckedChanged += new System.EventHandler(this.checkBoxAOMap_CheckedChanged);
            // 
            // checkBoxLightMap
            // 
            this.checkBoxLightMap.AutoSize = true;
            this.checkBoxLightMap.Location = new System.Drawing.Point(97, 42);
            this.checkBoxLightMap.Name = "checkBoxLightMap";
            this.checkBoxLightMap.Size = new System.Drawing.Size(72, 16);
            this.checkBoxLightMap.TabIndex = 4;
            this.checkBoxLightMap.Text = "光照贴图";
            this.checkBoxLightMap.UseVisualStyleBackColor = true;
            this.checkBoxLightMap.CheckedChanged += new System.EventHandler(this.checkBoxLightMap_CheckedChanged);
            // 
            // checkBoxSpecularMap
            // 
            this.checkBoxSpecularMap.AutoSize = true;
            this.checkBoxSpecularMap.Location = new System.Drawing.Point(7, 42);
            this.checkBoxSpecularMap.Name = "checkBoxSpecularMap";
            this.checkBoxSpecularMap.Size = new System.Drawing.Size(72, 16);
            this.checkBoxSpecularMap.TabIndex = 3;
            this.checkBoxSpecularMap.Text = "高光贴图";
            this.checkBoxSpecularMap.UseVisualStyleBackColor = true;
            this.checkBoxSpecularMap.CheckedChanged += new System.EventHandler(this.checkBoxSpecularMap_CheckedChanged);
            // 
            // checkBoxGlowMap
            // 
            this.checkBoxGlowMap.AutoSize = true;
            this.checkBoxGlowMap.Location = new System.Drawing.Point(7, 20);
            this.checkBoxGlowMap.Name = "checkBoxGlowMap";
            this.checkBoxGlowMap.Size = new System.Drawing.Size(84, 16);
            this.checkBoxGlowMap.TabIndex = 2;
            this.checkBoxGlowMap.Text = "自发光贴图";
            this.checkBoxGlowMap.UseVisualStyleBackColor = true;
            this.checkBoxGlowMap.CheckedChanged += new System.EventHandler(this.checkBoxGlowMap_CheckedChanged);
            // 
            // checkBoxNormalMap
            // 
            this.checkBoxNormalMap.AutoSize = true;
            this.checkBoxNormalMap.Location = new System.Drawing.Point(7, 64);
            this.checkBoxNormalMap.Name = "checkBoxNormalMap";
            this.checkBoxNormalMap.Size = new System.Drawing.Size(72, 16);
            this.checkBoxNormalMap.TabIndex = 1;
            this.checkBoxNormalMap.Text = "法线贴图";
            this.checkBoxNormalMap.UseVisualStyleBackColor = true;
            this.checkBoxNormalMap.CheckedChanged += new System.EventHandler(this.checkBoxNormal_CheckedChanged);
            // 
            // checkBoxDiffuseMap
            // 
            this.checkBoxDiffuseMap.AutoSize = true;
            this.checkBoxDiffuseMap.Location = new System.Drawing.Point(97, 20);
            this.checkBoxDiffuseMap.Name = "checkBoxDiffuseMap";
            this.checkBoxDiffuseMap.Size = new System.Drawing.Size(84, 16);
            this.checkBoxDiffuseMap.TabIndex = 0;
            this.checkBoxDiffuseMap.Text = "漫反射贴图";
            this.checkBoxDiffuseMap.UseVisualStyleBackColor = true;
            this.checkBoxDiffuseMap.CheckedChanged += new System.EventHandler(this.checkBoxDiffuseMap_CheckedChanged);
            // 
            // groupBoxLightting
            // 
            this.groupBoxLightting.Controls.Add(this.checkBoxSpecular);
            this.groupBoxLightting.Controls.Add(this.checkBoxDiffuse);
            this.groupBoxLightting.Location = new System.Drawing.Point(4, 264);
            this.groupBoxLightting.Name = "groupBoxLightting";
            this.groupBoxLightting.Size = new System.Drawing.Size(243, 46);
            this.groupBoxLightting.TabIndex = 1;
            this.groupBoxLightting.TabStop = false;
            this.groupBoxLightting.Text = "光照通道";
            // 
            // checkBoxSpecular
            // 
            this.checkBoxSpecular.AutoSize = true;
            this.checkBoxSpecular.Location = new System.Drawing.Point(96, 20);
            this.checkBoxSpecular.Name = "checkBoxSpecular";
            this.checkBoxSpecular.Size = new System.Drawing.Size(48, 16);
            this.checkBoxSpecular.TabIndex = 10;
            this.checkBoxSpecular.Text = "高光";
            this.checkBoxSpecular.UseVisualStyleBackColor = true;
            // 
            // checkBoxDiffuse
            // 
            this.checkBoxDiffuse.AutoSize = true;
            this.checkBoxDiffuse.Location = new System.Drawing.Point(6, 20);
            this.checkBoxDiffuse.Name = "checkBoxDiffuse";
            this.checkBoxDiffuse.Size = new System.Drawing.Size(60, 16);
            this.checkBoxDiffuse.TabIndex = 9;
            this.checkBoxDiffuse.Text = "漫反射";
            this.checkBoxDiffuse.UseVisualStyleBackColor = true;
            // 
            // groupBoxLight
            // 
            this.groupBoxLight.Controls.Add(this.checkBoxLocalLight);
            this.groupBoxLight.Controls.Add(this.checkBoxAssistLight);
            this.groupBoxLight.Controls.Add(this.checkBoxMainLight);
            this.groupBoxLight.Controls.Add(this.checkBoxAmbientLight);
            this.groupBoxLight.Location = new System.Drawing.Point(4, 316);
            this.groupBoxLight.Name = "groupBoxLight";
            this.groupBoxLight.Size = new System.Drawing.Size(243, 66);
            this.groupBoxLight.TabIndex = 2;
            this.groupBoxLight.TabStop = false;
            this.groupBoxLight.Text = "光源通道";
            // 
            // checkBoxLocalLight
            // 
            this.checkBoxLocalLight.AutoSize = true;
            this.checkBoxLocalLight.Location = new System.Drawing.Point(96, 42);
            this.checkBoxLocalLight.Name = "checkBoxLocalLight";
            this.checkBoxLocalLight.Size = new System.Drawing.Size(72, 16);
            this.checkBoxLocalLight.TabIndex = 13;
            this.checkBoxLocalLight.Text = "局部光源";
            this.checkBoxLocalLight.UseVisualStyleBackColor = true;
            // 
            // checkBoxAssistLight
            // 
            this.checkBoxAssistLight.AutoSize = true;
            this.checkBoxAssistLight.Location = new System.Drawing.Point(6, 42);
            this.checkBoxAssistLight.Name = "checkBoxAssistLight";
            this.checkBoxAssistLight.Size = new System.Drawing.Size(72, 16);
            this.checkBoxAssistLight.TabIndex = 12;
            this.checkBoxAssistLight.Text = "辅助光源";
            this.checkBoxAssistLight.UseVisualStyleBackColor = true;
            // 
            // checkBoxMainLight
            // 
            this.checkBoxMainLight.AutoSize = true;
            this.checkBoxMainLight.Location = new System.Drawing.Point(96, 20);
            this.checkBoxMainLight.Name = "checkBoxMainLight";
            this.checkBoxMainLight.Size = new System.Drawing.Size(60, 16);
            this.checkBoxMainLight.TabIndex = 11;
            this.checkBoxMainLight.Text = "主光源";
            this.checkBoxMainLight.UseVisualStyleBackColor = true;
            // 
            // checkBoxAmbientLight
            // 
            this.checkBoxAmbientLight.AutoSize = true;
            this.checkBoxAmbientLight.Location = new System.Drawing.Point(6, 20);
            this.checkBoxAmbientLight.Name = "checkBoxAmbientLight";
            this.checkBoxAmbientLight.Size = new System.Drawing.Size(60, 16);
            this.checkBoxAmbientLight.TabIndex = 10;
            this.checkBoxAmbientLight.Text = "环境光";
            this.checkBoxAmbientLight.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkBoxColorGrading);
            this.groupBox1.Controls.Add(this.checkBoxSoftParticle);
            this.groupBox1.Controls.Add(this.checkBoxHDR);
            this.groupBox1.Controls.Add(this.checkBoxGodRay);
            this.groupBox1.Controls.Add(this.checkBoxDof);
            this.groupBox1.Controls.Add(this.checkBoxMLAA);
            this.groupBox1.Controls.Add(this.checkBoxFog);
            this.groupBox1.Location = new System.Drawing.Point(3, 388);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(243, 91);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "后处理通道";
            // 
            // checkBoxColorGrading
            // 
            this.checkBoxColorGrading.AutoSize = true;
            this.checkBoxColorGrading.Location = new System.Drawing.Point(7, 42);
            this.checkBoxColorGrading.Name = "checkBoxColorGrading";
            this.checkBoxColorGrading.Size = new System.Drawing.Size(72, 16);
            this.checkBoxColorGrading.TabIndex = 14;
            this.checkBoxColorGrading.Text = "色调映射";
            this.checkBoxColorGrading.UseVisualStyleBackColor = true;
            this.checkBoxColorGrading.CheckedChanged += new System.EventHandler(this.checkBoxColorGrading_CheckedChanged);
            // 
            // checkBoxSoftParticle
            // 
            this.checkBoxSoftParticle.AutoSize = true;
            this.checkBoxSoftParticle.Location = new System.Drawing.Point(7, 20);
            this.checkBoxSoftParticle.Name = "checkBoxSoftParticle";
            this.checkBoxSoftParticle.Size = new System.Drawing.Size(60, 16);
            this.checkBoxSoftParticle.TabIndex = 13;
            this.checkBoxSoftParticle.Text = "软粒子";
            this.checkBoxSoftParticle.UseVisualStyleBackColor = true;
            // 
            // checkBoxHDR
            // 
            this.checkBoxHDR.AutoSize = true;
            this.checkBoxHDR.Location = new System.Drawing.Point(7, 64);
            this.checkBoxHDR.Name = "checkBoxHDR";
            this.checkBoxHDR.Size = new System.Drawing.Size(42, 16);
            this.checkBoxHDR.TabIndex = 12;
            this.checkBoxHDR.Text = "HDR";
            this.checkBoxHDR.UseVisualStyleBackColor = true;
            this.checkBoxHDR.CheckedChanged += new System.EventHandler(this.checkBoxHDR_CheckedChanged);
            // 
            // checkBoxGodRay
            // 
            this.checkBoxGodRay.AutoSize = true;
            this.checkBoxGodRay.Location = new System.Drawing.Point(145, 42);
            this.checkBoxGodRay.Name = "checkBoxGodRay";
            this.checkBoxGodRay.Size = new System.Drawing.Size(48, 16);
            this.checkBoxGodRay.TabIndex = 11;
            this.checkBoxGodRay.Text = "天光";
            this.checkBoxGodRay.UseVisualStyleBackColor = true;
            this.checkBoxGodRay.CheckedChanged += new System.EventHandler(this.checkBoxGodRay_CheckedChanged);
            // 
            // checkBoxDof
            // 
            this.checkBoxDof.AutoSize = true;
            this.checkBoxDof.Location = new System.Drawing.Point(81, 42);
            this.checkBoxDof.Name = "checkBoxDof";
            this.checkBoxDof.Size = new System.Drawing.Size(48, 16);
            this.checkBoxDof.TabIndex = 10;
            this.checkBoxDof.Text = "景深";
            this.checkBoxDof.UseVisualStyleBackColor = true;
            this.checkBoxDof.CheckedChanged += new System.EventHandler(this.checkBoxDof_CheckedChanged);
            // 
            // checkBoxMLAA
            // 
            this.checkBoxMLAA.AutoSize = true;
            this.checkBoxMLAA.Location = new System.Drawing.Point(144, 20);
            this.checkBoxMLAA.Name = "checkBoxMLAA";
            this.checkBoxMLAA.Size = new System.Drawing.Size(60, 16);
            this.checkBoxMLAA.TabIndex = 9;
            this.checkBoxMLAA.Text = "抗锯齿";
            this.checkBoxMLAA.UseVisualStyleBackColor = true;
            this.checkBoxMLAA.CheckedChanged += new System.EventHandler(this.checkBoxMLAA_CheckedChanged);
            // 
            // checkBoxFog
            // 
            this.checkBoxFog.AutoSize = true;
            this.checkBoxFog.Location = new System.Drawing.Point(81, 20);
            this.checkBoxFog.Name = "checkBoxFog";
            this.checkBoxFog.Size = new System.Drawing.Size(36, 16);
            this.checkBoxFog.TabIndex = 8;
            this.checkBoxFog.Text = "雾";
            this.checkBoxFog.UseVisualStyleBackColor = true;
            // 
            // groupBoxEntityType
            // 
            this.groupBoxEntityType.Location = new System.Drawing.Point(4, 0);
            this.groupBoxEntityType.Name = "groupBoxEntityType";
            this.groupBoxEntityType.Size = new System.Drawing.Size(239, 43);
            this.groupBoxEntityType.TabIndex = 0;
            this.groupBoxEntityType.TabStop = false;
            this.groupBoxEntityType.Text = "实体类型";
            // 
            // groupBoxSelection
            // 
            this.groupBoxSelection.Location = new System.Drawing.Point(3, 560);
            this.groupBoxSelection.Name = "groupBoxSelection";
            this.groupBoxSelection.Size = new System.Drawing.Size(242, 62);
            this.groupBoxSelection.TabIndex = 5;
            this.groupBoxSelection.TabStop = false;
            this.groupBoxSelection.Text = "选择设置";
            // 
            // groupBox2
            // 
            this.groupBox2.Location = new System.Drawing.Point(4, 43);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(242, 49);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "辅助标记类型";
            // 
            // groupBoxRenderLevel
            // 
            this.groupBoxRenderLevel.Location = new System.Drawing.Point(4, 92);
            this.groupBoxRenderLevel.Name = "groupBoxRenderLevel";
            this.groupBoxRenderLevel.Size = new System.Drawing.Size(242, 49);
            this.groupBoxRenderLevel.TabIndex = 2;
            this.groupBoxRenderLevel.TabStop = false;
            this.groupBoxRenderLevel.Text = "渲染等级";
            // 
            // groupBox3
            // 
            this.groupBox3.Location = new System.Drawing.Point(4, 507);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(242, 47);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "效果等级";
            // 
            // SettingPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBoxRenderLevel);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBoxSelection);
            this.Controls.Add(this.groupBoxEntityType);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBoxLight);
            this.Controls.Add(this.groupBoxLightting);
            this.Controls.Add(this.groupBoxMappingChannel);
            this.Name = "SettingPanel";
            this.Size = new System.Drawing.Size(250, 631);
            this.groupBoxMappingChannel.ResumeLayout(false);
            this.groupBoxMappingChannel.PerformLayout();
            this.groupBoxLightting.ResumeLayout(false);
            this.groupBoxLightting.PerformLayout();
            this.groupBoxLight.ResumeLayout(false);
            this.groupBoxLight.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxMappingChannel;
        private System.Windows.Forms.GroupBox groupBoxLightting;
        private System.Windows.Forms.GroupBox groupBoxLight;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxSpecularMap;
        private System.Windows.Forms.CheckBox checkBoxGlowMap;
        private System.Windows.Forms.CheckBox checkBoxNormalMap;
        private System.Windows.Forms.CheckBox checkBoxDiffuseMap;
        private System.Windows.Forms.CheckBox checkBoxAOMap;
        private System.Windows.Forms.CheckBox checkBoxLightMap;
        private System.Windows.Forms.CheckBox checkBoxDetailNormalMap;
        private System.Windows.Forms.CheckBox checkBoxEnvironmentMap;
        private System.Windows.Forms.GroupBox groupBoxEntityType;
        private System.Windows.Forms.GroupBox groupBoxSelection;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBoxRenderLevel;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox checkBoxSoftParticle;
        private System.Windows.Forms.CheckBox checkBoxHDR;
        private System.Windows.Forms.CheckBox checkBoxGodRay;
        private System.Windows.Forms.CheckBox checkBoxDof;
        private System.Windows.Forms.CheckBox checkBoxMLAA;
        private System.Windows.Forms.CheckBox checkBoxFog;
        private System.Windows.Forms.CheckBox checkBoxSpecular;
        private System.Windows.Forms.CheckBox checkBoxDiffuse;
        private System.Windows.Forms.CheckBox checkBoxAssistLight;
        private System.Windows.Forms.CheckBox checkBoxMainLight;
        private System.Windows.Forms.CheckBox checkBoxAmbientLight;
        private System.Windows.Forms.CheckBox checkBoxLocalLight;
        private System.Windows.Forms.CheckBox checkBoxColorGrading;
    }
}
