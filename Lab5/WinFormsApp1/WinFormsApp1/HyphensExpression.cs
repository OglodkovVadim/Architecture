using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1
{
    internal class HyphensExpression : IExpression
    {
        IExpression expression;

        public HyphensExpression(IExpression expression)
        {
            this.expression = expression;
        }

        public string Interpret(string context)
        {
            return expression.Interpret(context).Replace("-", "—-");
        }
    }
}
