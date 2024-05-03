using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using static System.Net.Mime.MediaTypeNames;

namespace WinFormsApp1
{
    internal class ExtraSpaceBracket : IExpression
    {
        IExpression expression;
        public ExtraSpaceBracket(IExpression expression)
        {
            this.expression=expression; 
        }

        public string Interpret(string context)
        {
            string buf = Regex.Replace(context, @"\(\s+", "(");
            buf = Regex.Replace(buf, @"\s+\)", ")");
            buf = Regex.Replace(buf, @"\s+,", ",");
            buf = Regex.Replace(buf, @"\s+\.", ".");
            return buf;
        }
    }
}
