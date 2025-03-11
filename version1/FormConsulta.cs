using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics.Contracts;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.Remoting.Channels;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace version1
{
    public partial class FormConsulta: Form
    {
        public FormConsulta()
        {
            InitializeComponent();
        }
        Socket server;
        string id;
        string idP;
        string contra;

        public void setServer(Socket server)
        {
            this.server = server;
        }
        public void setId(string id)
        {
            this.id = id;
        }
        public void setContra(string contra)
        {
            this.contra = contra;
        }



        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            label1.Visible = true;
            idPbox.Visible = true;
            button1.Visible = true;
        }

        private void Consulta_Load(object sender, EventArgs e)
        {
            loginLabel.Visible = true;
            loginLabel.Text = "ID: " + id;
            }

        private void button1_Click(object sender, EventArgs e)
        {
            //guardar idP
            idP = idPbox.Text;

            string mensaje = "3/" + id + "/" + idP;
            // Enviamos al servidor el registro tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor si es 0 todo ha ido bien
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            string[] partes = mensaje.Split('/');

            label1.Visible = false;
            idPbox.Visible = false;
            button1.Visible = false;
            MessageBox.Show("Han jugado las siguientes personas: " + string.Join(", ", partes));
        }

        private void conQuePersonasHeJugadoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string mensaje = "4/" + id + "/"+ contra;
            // Enviamos al servidor el registro tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            string[] partes = mensaje.Split('/');

            MessageBox.Show("Has jugado con las siguientes personas: " + string.Join(", ", partes));
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int min = Convert.ToInt32(minBox.Text);
            int max = Convert.ToInt32(maxBox.Text);

            string mensaje = "5/" + id + "/" + contra + "/" + min + "/" + max;
            // Enviamos al servidor el registro tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor si es 0 todo ha ido bien
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];


            label2.Visible = false;
            minBox.Visible = false;
            maxBox.Visible = false;
            button2.Visible = false;
            MessageBox.Show("Se han jugado: " + mensaje);
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void cuantasPartidasSeHanJugadoEnXTiempoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label2.Visible = true;
            minBox.Visible = true;
            maxBox.Visible = true;
            button2.Visible = true;
        }
    }
}
