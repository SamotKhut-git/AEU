using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Inventory_management_system
{
    public partial class sample : Form
    {
        public sample()
        {
            InitializeComponent();
        }

        private void btnSign_click(object sender, EventArgs e)
        {
            
        }
        private void btnLog_click(object sender, EventArgs e)
        {
            frmMain frmMain = new frmMain();

            this.Hide();

            frmMain.Show();

           
        }

       
    }
}
