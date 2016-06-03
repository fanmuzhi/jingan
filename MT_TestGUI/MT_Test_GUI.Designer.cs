namespace MT_TestGUI_Demo
{
    partial class MT_Analysis_GUI
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Main_GUI_Tab = new System.Windows.Forms.TabControl();
            this.Main_GUI = new System.Windows.Forms.TabPage();
            this.Serial_Num_Label = new System.Windows.Forms.Label();
            this.Config_File_Selected = new System.Windows.Forms.TextBox();
            this.Module_SerialNum = new System.Windows.Forms.TextBox();
            this.TestStatus_TextBox = new System.Windows.Forms.TextBox();
            this.SelectLogFile_Button = new System.Windows.Forms.Button();
            this.LogFile_Path = new System.Windows.Forms.TextBox();
            this.Test_Settings_Group = new System.Windows.Forms.GroupBox();
            this.Invalidate_CheckBox = new System.Windows.Forms.CheckBox();
            this.QA_Mode_CheckBox = new System.Windows.Forms.CheckBox();
            this.Subtest_Listview = new System.Windows.Forms.ListView();
            this.SubTest_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.TestStatus_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Data_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Select_MT_Config_Button = new System.Windows.Forms.Button();
            this.Continue_Test_Button = new System.Windows.Forms.Button();
            this.Start_Test_Button = new System.Windows.Forms.Button();
            this.Fake_Finger_Label = new System.Windows.Forms.Label();
            this.NoFinger_Label = new System.Windows.Forms.Label();
            this.FakeFinger_Image = new System.Windows.Forms.PictureBox();
            this.NoFinger_Image = new System.Windows.Forms.PictureBox();
            this.BinCode_Page = new System.Windows.Forms.TabPage();
            this.BinCodes_Listview = new System.Windows.Forms.ListView();
            this.BinCode_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.FailureMode_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.TestName_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Main_GUI_Tab.SuspendLayout();
            this.Main_GUI.SuspendLayout();
            this.Test_Settings_Group.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FakeFinger_Image)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NoFinger_Image)).BeginInit();
            this.BinCode_Page.SuspendLayout();
            this.SuspendLayout();
            // 
            // Main_GUI_Tab
            // 
            this.Main_GUI_Tab.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.Main_GUI_Tab.Controls.Add(this.Main_GUI);
            this.Main_GUI_Tab.Controls.Add(this.BinCode_Page);
            this.Main_GUI_Tab.Location = new System.Drawing.Point(12, 12);
            this.Main_GUI_Tab.Name = "Main_GUI_Tab";
            this.Main_GUI_Tab.SelectedIndex = 0;
            this.Main_GUI_Tab.Size = new System.Drawing.Size(560, 565);
            this.Main_GUI_Tab.TabIndex = 0;
            // 
            // Main_GUI
            // 
            this.Main_GUI.Controls.Add(this.Serial_Num_Label);
            this.Main_GUI.Controls.Add(this.Config_File_Selected);
            this.Main_GUI.Controls.Add(this.Module_SerialNum);
            this.Main_GUI.Controls.Add(this.TestStatus_TextBox);
            this.Main_GUI.Controls.Add(this.SelectLogFile_Button);
            this.Main_GUI.Controls.Add(this.LogFile_Path);
            this.Main_GUI.Controls.Add(this.Test_Settings_Group);
            this.Main_GUI.Controls.Add(this.Subtest_Listview);
            this.Main_GUI.Controls.Add(this.Select_MT_Config_Button);
            this.Main_GUI.Controls.Add(this.Continue_Test_Button);
            this.Main_GUI.Controls.Add(this.Start_Test_Button);
            this.Main_GUI.Controls.Add(this.Fake_Finger_Label);
            this.Main_GUI.Controls.Add(this.NoFinger_Label);
            this.Main_GUI.Controls.Add(this.FakeFinger_Image);
            this.Main_GUI.Controls.Add(this.NoFinger_Image);
            this.Main_GUI.Location = new System.Drawing.Point(4, 22);
            this.Main_GUI.Name = "Main_GUI";
            this.Main_GUI.Padding = new System.Windows.Forms.Padding(3);
            this.Main_GUI.Size = new System.Drawing.Size(552, 539);
            this.Main_GUI.TabIndex = 0;
            this.Main_GUI.Text = "Test Summary";
            this.Main_GUI.UseVisualStyleBackColor = true;
            // 
            // Serial_Num_Label
            // 
            this.Serial_Num_Label.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.Serial_Num_Label.AutoSize = true;
            this.Serial_Num_Label.Location = new System.Drawing.Point(318, 75);
            this.Serial_Num_Label.Name = "Serial_Num_Label";
            this.Serial_Num_Label.Size = new System.Drawing.Size(49, 13);
            this.Serial_Num_Label.TabIndex = 13;
            this.Serial_Num_Label.Text = "Serial # :";
            // 
            // Config_File_Selected
            // 
            this.Config_File_Selected.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.Config_File_Selected.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Config_File_Selected.Location = new System.Drawing.Point(162, 6);
            this.Config_File_Selected.Name = "Config_File_Selected";
            this.Config_File_Selected.ReadOnly = true;
            this.Config_File_Selected.Size = new System.Drawing.Size(384, 20);
            this.Config_File_Selected.TabIndex = 3;
            // 
            // Module_SerialNum
            // 
            this.Module_SerialNum.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.Module_SerialNum.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Module_SerialNum.Location = new System.Drawing.Point(373, 72);
            this.Module_SerialNum.Name = "Module_SerialNum";
            this.Module_SerialNum.ReadOnly = true;
            this.Module_SerialNum.Size = new System.Drawing.Size(173, 20);
            this.Module_SerialNum.TabIndex = 12;
            // 
            // TestStatus_TextBox
            // 
            this.TestStatus_TextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.TestStatus_TextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TestStatus_TextBox.Font = new System.Drawing.Font("Arial Narrow", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TestStatus_TextBox.Location = new System.Drawing.Point(318, 33);
            this.TestStatus_TextBox.Multiline = true;
            this.TestStatus_TextBox.Name = "TestStatus_TextBox";
            this.TestStatus_TextBox.ReadOnly = true;
            this.TestStatus_TextBox.Size = new System.Drawing.Size(228, 33);
            this.TestStatus_TextBox.TabIndex = 11;
            this.TestStatus_TextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // SelectLogFile_Button
            // 
            this.SelectLogFile_Button.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.SelectLogFile_Button.BackColor = System.Drawing.Color.Gainsboro;
            this.SelectLogFile_Button.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.SelectLogFile_Button.Location = new System.Drawing.Point(6, 504);
            this.SelectLogFile_Button.Name = "SelectLogFile_Button";
            this.SelectLogFile_Button.Size = new System.Drawing.Size(150, 23);
            this.SelectLogFile_Button.TabIndex = 10;
            this.SelectLogFile_Button.Text = "Select Log File";
            this.SelectLogFile_Button.UseVisualStyleBackColor = false;
            this.SelectLogFile_Button.Click += new System.EventHandler(this.SelectLogFile_Button_Click);
            // 
            // LogFile_Path
            // 
            this.LogFile_Path.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.LogFile_Path.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LogFile_Path.Location = new System.Drawing.Point(162, 506);
            this.LogFile_Path.Name = "LogFile_Path";
            this.LogFile_Path.Size = new System.Drawing.Size(384, 20);
            this.LogFile_Path.TabIndex = 9;
            // 
            // Test_Settings_Group
            // 
            this.Test_Settings_Group.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Test_Settings_Group.Controls.Add(this.Invalidate_CheckBox);
            this.Test_Settings_Group.Controls.Add(this.QA_Mode_CheckBox);
            this.Test_Settings_Group.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Test_Settings_Group.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Test_Settings_Group.Location = new System.Drawing.Point(6, 434);
            this.Test_Settings_Group.Name = "Test_Settings_Group";
            this.Test_Settings_Group.Size = new System.Drawing.Size(150, 64);
            this.Test_Settings_Group.TabIndex = 7;
            this.Test_Settings_Group.TabStop = false;
            this.Test_Settings_Group.Text = "Test Settings";
            // 
            // Invalidate_CheckBox
            // 
            this.Invalidate_CheckBox.AutoSize = true;
            this.Invalidate_CheckBox.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Invalidate_CheckBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Invalidate_CheckBox.Location = new System.Drawing.Point(6, 41);
            this.Invalidate_CheckBox.Name = "Invalidate_CheckBox";
            this.Invalidate_CheckBox.Size = new System.Drawing.Size(70, 17);
            this.Invalidate_CheckBox.TabIndex = 6;
            this.Invalidate_CheckBox.Text = "Invalidate";
            this.Invalidate_CheckBox.UseVisualStyleBackColor = true;
            // 
            // QA_Mode_CheckBox
            // 
            this.QA_Mode_CheckBox.AutoSize = true;
            this.QA_Mode_CheckBox.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.QA_Mode_CheckBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.QA_Mode_CheckBox.Location = new System.Drawing.Point(6, 19);
            this.QA_Mode_CheckBox.Name = "QA_Mode_CheckBox";
            this.QA_Mode_CheckBox.Size = new System.Drawing.Size(69, 17);
            this.QA_Mode_CheckBox.TabIndex = 6;
            this.QA_Mode_CheckBox.Text = "QA Mode";
            this.QA_Mode_CheckBox.UseVisualStyleBackColor = true;
            // 
            // Subtest_Listview
            // 
            this.Subtest_Listview.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.Subtest_Listview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Subtest_Listview.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.SubTest_Column,
            this.TestStatus_Column,
            this.Data_Column});
            this.Subtest_Listview.GridLines = true;
            this.Subtest_Listview.Location = new System.Drawing.Point(162, 98);
            this.Subtest_Listview.Name = "Subtest_Listview";
            this.Subtest_Listview.Size = new System.Drawing.Size(384, 402);
            this.Subtest_Listview.TabIndex = 5;
            this.Subtest_Listview.UseCompatibleStateImageBehavior = false;
            this.Subtest_Listview.View = System.Windows.Forms.View.Details;
            // 
            // SubTest_Column
            // 
            this.SubTest_Column.Text = "SubTest";
            this.SubTest_Column.Width = 150;
            // 
            // TestStatus_Column
            // 
            this.TestStatus_Column.Text = "Status";
            this.TestStatus_Column.Width = 48;
            // 
            // Data_Column
            // 
            this.Data_Column.Text = "Data";
            this.Data_Column.Width = 152;
            // 
            // Select_MT_Config_Button
            // 
            this.Select_MT_Config_Button.BackColor = System.Drawing.Color.Gainsboro;
            this.Select_MT_Config_Button.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Select_MT_Config_Button.Location = new System.Drawing.Point(6, 4);
            this.Select_MT_Config_Button.Name = "Select_MT_Config_Button";
            this.Select_MT_Config_Button.Size = new System.Drawing.Size(150, 23);
            this.Select_MT_Config_Button.TabIndex = 4;
            this.Select_MT_Config_Button.Text = "Select Config File";
            this.Select_MT_Config_Button.UseVisualStyleBackColor = false;
            this.Select_MT_Config_Button.Click += new System.EventHandler(this.Select_MT_Config_Button_Click);
            // 
            // Continue_Test_Button
            // 
            this.Continue_Test_Button.BackColor = System.Drawing.Color.Gainsboro;
            this.Continue_Test_Button.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Continue_Test_Button.Font = new System.Drawing.Font("Arial", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Continue_Test_Button.Location = new System.Drawing.Point(162, 33);
            this.Continue_Test_Button.Name = "Continue_Test_Button";
            this.Continue_Test_Button.Size = new System.Drawing.Size(150, 59);
            this.Continue_Test_Button.TabIndex = 2;
            this.Continue_Test_Button.Text = "Continue";
            this.Continue_Test_Button.UseVisualStyleBackColor = false;
            this.Continue_Test_Button.Click += new System.EventHandler(this.Continue_Test_Button_Click);
            // 
            // Start_Test_Button
            // 
            this.Start_Test_Button.BackColor = System.Drawing.Color.Gainsboro;
            this.Start_Test_Button.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Start_Test_Button.Font = new System.Drawing.Font("Arial", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Start_Test_Button.Location = new System.Drawing.Point(6, 33);
            this.Start_Test_Button.Name = "Start_Test_Button";
            this.Start_Test_Button.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.Start_Test_Button.Size = new System.Drawing.Size(150, 59);
            this.Start_Test_Button.TabIndex = 2;
            this.Start_Test_Button.Text = "Start Test";
            this.Start_Test_Button.UseVisualStyleBackColor = false;
            this.Start_Test_Button.Click += new System.EventHandler(this.Start_Test_Button_Click);
            // 
            // Fake_Finger_Label
            // 
            this.Fake_Finger_Label.AutoSize = true;
            this.Fake_Finger_Label.Location = new System.Drawing.Point(6, 391);
            this.Fake_Finger_Label.Name = "Fake_Finger_Label";
            this.Fake_Finger_Label.Size = new System.Drawing.Size(95, 13);
            this.Fake_Finger_Label.TabIndex = 1;
            this.Fake_Finger_Label.Text = "Fake Finger Image";
            // 
            // NoFinger_Label
            // 
            this.NoFinger_Label.AutoSize = true;
            this.NoFinger_Label.Location = new System.Drawing.Point(6, 235);
            this.NoFinger_Label.Name = "NoFinger_Label";
            this.NoFinger_Label.Size = new System.Drawing.Size(85, 13);
            this.NoFinger_Label.TabIndex = 1;
            this.NoFinger_Label.Text = "No Finger Image";
            // 
            // FakeFinger_Image
            // 
            this.FakeFinger_Image.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FakeFinger_Image.Location = new System.Drawing.Point(6, 254);
            this.FakeFinger_Image.Name = "FakeFinger_Image";
            this.FakeFinger_Image.Size = new System.Drawing.Size(150, 150);
            this.FakeFinger_Image.TabIndex = 0;
            this.FakeFinger_Image.TabStop = false;
            // 
            // NoFinger_Image
            // 
            this.NoFinger_Image.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.NoFinger_Image.Location = new System.Drawing.Point(6, 98);
            this.NoFinger_Image.Name = "NoFinger_Image";
            this.NoFinger_Image.Size = new System.Drawing.Size(150, 150);
            this.NoFinger_Image.TabIndex = 0;
            this.NoFinger_Image.TabStop = false;
            // 
            // BinCode_Page
            // 
            this.BinCode_Page.Controls.Add(this.BinCodes_Listview);
            this.BinCode_Page.Location = new System.Drawing.Point(4, 22);
            this.BinCode_Page.Name = "BinCode_Page";
            this.BinCode_Page.Padding = new System.Windows.Forms.Padding(3);
            this.BinCode_Page.Size = new System.Drawing.Size(552, 539);
            this.BinCode_Page.TabIndex = 1;
            this.BinCode_Page.Text = "Bin Codes";
            this.BinCode_Page.UseVisualStyleBackColor = true;
            // 
            // BinCodes_Listview
            // 
            this.BinCodes_Listview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.BinCodes_Listview.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.BinCode_Column,
            this.TestName_Column,
            this.FailureMode_Column});
            this.BinCodes_Listview.GridLines = true;
            this.BinCodes_Listview.Location = new System.Drawing.Point(6, 6);
            this.BinCodes_Listview.Name = "BinCodes_Listview";
            this.BinCodes_Listview.Size = new System.Drawing.Size(540, 527);
            this.BinCodes_Listview.TabIndex = 0;
            this.BinCodes_Listview.UseCompatibleStateImageBehavior = false;
            this.BinCodes_Listview.View = System.Windows.Forms.View.Details;
            // 
            // BinCode_Column
            // 
            this.BinCode_Column.Text = "Bin Code";
            // 
            // FailureMode_Column
            // 
            this.FailureMode_Column.Text = "Failure Mode";
            this.FailureMode_Column.Width = 376;
            // 
            // TestName_Column
            // 
            this.TestName_Column.Text = "Test Name";
            this.TestName_Column.Width = 72;
            // 
            // MT_Analysis_GUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 582);
            this.Controls.Add(this.Main_GUI_Tab);
            this.MinimumSize = new System.Drawing.Size(600, 620);
            this.Name = "MT_Analysis_GUI";
            this.Text = "Module Test Data Analysis";
            this.Main_GUI_Tab.ResumeLayout(false);
            this.Main_GUI.ResumeLayout(false);
            this.Main_GUI.PerformLayout();
            this.Test_Settings_Group.ResumeLayout(false);
            this.Test_Settings_Group.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FakeFinger_Image)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NoFinger_Image)).EndInit();
            this.BinCode_Page.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl Main_GUI_Tab;
        private System.Windows.Forms.TabPage Main_GUI;
        private System.Windows.Forms.Button Select_MT_Config_Button;
        private System.Windows.Forms.TextBox Config_File_Selected;
        private System.Windows.Forms.Button Continue_Test_Button;
        private System.Windows.Forms.Button Start_Test_Button;
        private System.Windows.Forms.Label Fake_Finger_Label;
        private System.Windows.Forms.Label NoFinger_Label;
        private System.Windows.Forms.PictureBox FakeFinger_Image;
        private System.Windows.Forms.PictureBox NoFinger_Image;
        private System.Windows.Forms.ListView Subtest_Listview;
        private System.Windows.Forms.ColumnHeader SubTest_Column;
        private System.Windows.Forms.ColumnHeader TestStatus_Column;
        private System.Windows.Forms.GroupBox Test_Settings_Group;
        private System.Windows.Forms.CheckBox Invalidate_CheckBox;
        private System.Windows.Forms.CheckBox QA_Mode_CheckBox;
        private System.Windows.Forms.Button SelectLogFile_Button;
        private System.Windows.Forms.TextBox LogFile_Path;
        private System.Windows.Forms.TextBox TestStatus_TextBox;
        private System.Windows.Forms.TextBox Module_SerialNum;
        private System.Windows.Forms.Label Serial_Num_Label;
        private System.Windows.Forms.ColumnHeader Data_Column;
        private System.Windows.Forms.TabPage BinCode_Page;
        private System.Windows.Forms.ListView BinCodes_Listview;
        private System.Windows.Forms.ColumnHeader BinCode_Column;
        private System.Windows.Forms.ColumnHeader TestName_Column;
        private System.Windows.Forms.ColumnHeader FailureMode_Column;
    }
}

