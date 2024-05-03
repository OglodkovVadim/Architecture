using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

namespace WinFormsApp1
{
    internal class ExtraSpacesExpression : IExpression
    {
        IExpression expression;
        
        public ExtraSpacesExpression(IExpression expression)
        {
            this.expression = expression;
        }

        public string Interpret(string context)
        {
            return Regex.Replace(expression.Interpret(context).Trim(),
                @"\s+", " ");
        }
    }
}
