using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace WinFormsApp1
{
    internal class ExtraNewLineExpression : IExpression
    {
        public ExtraNewLineExpression()
        {
        }

        public string Interpret(string context)
        {
            return Regex.Replace(context, @"\n{2,}", "\n");
        }
    }
}