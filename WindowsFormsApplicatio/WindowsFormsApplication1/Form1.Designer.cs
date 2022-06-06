using System;
using System.IO.Ports;

namespace WindowsFormsApplication1
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
                components.Dispose();
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.panelM = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.buttonOpenPort = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.buttonFromSlave2 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.buttonFromSlave1 = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.panelM.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelM
            // 
            this.panelM.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panelM.Controls.Add(this.label1);
            this.panelM.Controls.Add(this.comboBox1);
            this.panelM.Controls.Add(this.buttonOpenPort);
            this.panelM.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelM.Location = new System.Drawing.Point(0, 0);
            this.panelM.Name = "panelM";
            this.panelM.Size = new System.Drawing.Size(287, 39);
            this.panelM.TabIndex = 19;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.Location = new System.Drawing.Point(3, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 18);
            this.label1.TabIndex = 7;
            this.label1.Text = "COM порт";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.comboBox1.Location = new System.Drawing.Point(92, 2);
            this.comboBox1.MaxDropDownItems = 10;
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(88, 26);
            this.comboBox1.TabIndex = 6;
            this.comboBox1.Click += new System.EventHandler(this.comboBox1_Click);
            // 
            // buttonOpenPort
            // 
            this.buttonOpenPort.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonOpenPort.Location = new System.Drawing.Point(185, 3);
            this.buttonOpenPort.Name = "buttonOpenPort";
            this.buttonOpenPort.Size = new System.Drawing.Size(88, 25);
            this.buttonOpenPort.TabIndex = 5;
            this.buttonOpenPort.Tag = "1";
            this.buttonOpenPort.Text = "Відкрити";
            this.buttonOpenPort.UseVisualStyleBackColor = true;
            this.buttonOpenPort.Click += new System.EventHandler(this.buttonOpenPort_Click);
            // 
            // buttonFromSlave2
            // 
            this.buttonFromSlave2.BackColor = System.Drawing.Color.Transparent;
            this.buttonFromSlave2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonFromSlave2.ForeColor = System.Drawing.Color.Black;
            this.buttonFromSlave2.Location = new System.Drawing.Point(148, 176);
            this.buttonFromSlave2.Name = "buttonFromSlave2";
            this.buttonFromSlave2.Size = new System.Drawing.Size(116, 49);
            this.buttonFromSlave2.TabIndex = 24;
            this.buttonFromSlave2.Text = "Slave2";
            this.buttonFromSlave2.UseVisualStyleBackColor = false;
            this.buttonFromSlave2.Visible = false;
            this.buttonFromSlave2.Click += new System.EventHandler(this.buttonFromSlave2_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 50);
            this.textBox1.Margin = new System.Windows.Forms.Padding(2);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(253, 20);
            this.textBox1.TabIndex = 21;
            // 
            // buttonFromSlave1
            // 
            this.buttonFromSlave1.BackColor = System.Drawing.Color.Transparent;
            this.buttonFromSlave1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonFromSlave1.ForeColor = System.Drawing.Color.Black;
            this.buttonFromSlave1.Location = new System.Drawing.Point(6, 176);
            this.buttonFromSlave1.Name = "buttonFromSlave1";
            this.buttonFromSlave1.Size = new System.Drawing.Size(116, 49);
            this.buttonFromSlave1.TabIndex = 23;
            this.buttonFromSlave1.Text = "Slave1";
            this.buttonFromSlave1.UseVisualStyleBackColor = false;
            this.buttonFromSlave1.Visible = false;
            this.buttonFromSlave1.Click += new System.EventHandler(this.buttonFromSlave1_Click);
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(12, 98);
            this.textBox3.Margin = new System.Windows.Forms.Padding(2);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(253, 20);
            this.textBox3.TabIndex = 26;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(11, 122);
            this.textBox4.Margin = new System.Windows.Forms.Padding(2);
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size(253, 20);
            this.textBox4.TabIndex = 27;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(12, 74);
            this.textBox2.Margin = new System.Windows.Forms.Padding(2);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(253, 20);
            this.textBox2.TabIndex = 22;
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(12, 147);
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(252, 20);
            this.textBox5.TabIndex = 28;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(287, 233);
            this.Controls.Add(this.textBox5);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.buttonFromSlave2);
            this.Controls.Add(this.buttonFromSlave1);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.panelM);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panelM.ResumeLayout(false);
            this.panelM.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panelM;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button buttonOpenPort;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button buttonFromSlave2;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button buttonFromSlave1;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox5;
    }
}

