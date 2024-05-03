namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        IExpression expression = 
            new ExtraSpacesExpression(
                new HyphensExpression(
                    new QuotateExpression(
                        new ExtraSpaceBracket(
                            new ExtraNewLineExpression()
                        )
                    )
                )
            );

        public Form1()
        {
            InitializeComponent();
        }

        private void check_Click(object sender, EventArgs e)
        {
            text.Text = expression.Interpret(text.Text);
        }
    }
}
