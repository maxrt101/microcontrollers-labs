using System;
using System.Data;
using System.Text;
using System.Drawing;
using System.IO.Ports;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        const int SERIAL_SPEED = 19200;

        const byte SLAVE1_ADDRESS = 0xE9;
        const byte SLAVE2_ADDRESS = 0xA7;

        const byte COMMAND_BYTE = 0xB1;

        const int CRC_INIT = 0xffff;

        static const ushort[] CRC_CCITT_TABLE = new ushort[] {
            0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
            0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
            0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
            0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
            0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
            0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
            0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
            0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
            0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
            0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
            0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
            0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
            0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
            0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
            0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
            0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
            0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
            0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
            0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
            0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
            0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
            0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
            0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
            0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
            0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
            0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
            0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
            0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
            0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
            0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
            0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
            0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
        };

        public Form1() {
            InitializeComponent();
            serialPort1.BaudRate = SERIAL_SPEED;
        }

        private void comboBox1_Click(object sender, EventArgs e) {
            int num;
            string[] ports = SerialPort.GetPortNames().OrderBy(
                a => a.Length > 3 && int.TryParse(a.Substring(3), out num) ? num : 0
            ).ToArray();
            comboBox1.Items.Clear();
            comboBox1.Items.AddRange(ports);
        }

        private void buttonOpenPort_Click(object sender, EventArgs e) {
            if (!serialPort1.IsOpen) {
                try {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                    buttonOpenPort.Text = "Close";
                    comboBox1.Enabled = false;
                    buttonFromSlave1.Visible = true;
                    buttonFromSlave2.Visible = true;
                } catch {
                    MessageBox.Show("Port " + comboBox1.Text + " is invalid!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            } else {
                serialPort1.Close();
                buttonOpenPort.Text = "Open";
                comboBox1.Enabled = true;
                buttonFromSlave1.Visible = false;
                buttonFromSlave2.Visible = false;
            }
        }

        private void buttonFromSlave1_Click(object sender, EventArgs e) {
            byte[] payload = new byte[] { SLAVE1_ADDRESS, COMMAND_BYTE };
            serialPort1.Write(payload, 0, 2);


            List<byte> inputBytes = new List<byte>();
            int expectedSize = serialPort1.ReadByte();

            for (int i = 0; i < expectedSize; i++) {
                inputBytes.Add((byte) serialPort1.ReadByte());
            }

            String input = System.Text.Encoding.ASCII.GetString(inputBytes.ToArray());

            ushort transferredChecksum = CombineBytes((byte) serialPort1.ReadByte(), (byte) serialPort1.ReadByte());
            ushort calculatedChecksum  = CalculateCRC(inputBytes, DATA1_SIZE);

            if (transferredChecksum == calculatedChecksum) {
                textBox1.Text = input;
            } else {
                textBox1.Text = "Bad data! recv:" + transferredChecksum.ToString() + " calc:" + calculatedChecksum.ToString();
            }
        }

        private void buttonFromSlave2_Click(object sender, EventArgs e) {
          byte[] payload = new byte[] { SLAVE2_ADDRESS, COMMAND_BYTE };
          serialPort1.Write(payload, 0, 2);

          TextBox[] textBoxes = { textBox1, textBox2, textBox3, textBox4, textBox5 };

          List<byte> inputBytes = new List<byte>();
          int expectedSize = serialPort1.ReadByte();

          for (int i = 0; i < expectedSize; i++) {
              inputBytes.Add((byte) serialPort1.ReadByte());
          }

          String input = System.Text.Encoding.ASCII.GetString(inputBytes.ToArray());

          ushort transferredChecksum = CombineBytes((byte) serialPort1.ReadByte(), (byte) serialPort1.ReadByte());
          ushort calculatedChecksum  = CalculateCRC(inputBytes, DATA1_SIZE);

          if (transferredChecksum == calculatedChecksum) {
              textBox1.Text = input;
          } else {
              textBox1.Text = "Bad data! recv:" + transferredChecksum.ToString() + " calc:" + calculatedChecksum.ToString();
          }
        }

        public static ushort CalculateCRC(List<byte> buffer, int length) {
            ushort tmp = 0;
            ushort crc = CRC_INIT;

            for (int i = 0; i < size ; i++) {
                tmp = (ushort) ((crc >> 8) ^ buffer[i]);
                crc = (ushort) ((crc << 8) ^ CRC_CCITT_TABLE[tmp]);
            }

            return crc;
        }

        public int CombineBytes(byte b1, byte b2) {
            return (ushort)b1 << 8 | b2;
        }

        private void label1_Click(object sender, EventArgs e) {}

        private void Form1_Load(object sender, EventArgs e) {}

        private void textBox1_TextChanged(object sender, EventArgs e) {}
    }
}
