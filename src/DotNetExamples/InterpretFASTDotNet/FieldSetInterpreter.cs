using System;
using System.Collections.Generic;
using System.Text;
using QuickFAST.DotNet;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class FieldSetInterpreter
        {
            String indent_ = "";

            public void interpret(QuickFAST.DotNet.DNFieldSet fieldSet)
            {
                if (!silent_)
                {
                    formatFieldSet(fieldSet);
                }
            }

            public void formatFieldSet(QuickFAST.DotNet.DNFieldSet fieldSet)
            {
                int size = fieldSet.Count;
                // There are two equivalent ways to iterate through the fields; by index or by foreach (enumerator)
                //   for (int nField = 0; nField < size; ++nField)
                //   {
                //      DNField field = fieldSet.getField(nField);
                // is equivalent to:
                //    foreach (DNField field in fieldSet)
                //    {
                // You can also find fields by name
                // This uses the unqualified local name.  It does not use field namespace
                //   DNField field = fieldSet.findFieldByName("hello");
                // And this one uses the namespace
                //   DNField field = fieldSet.findFieldByQualifiedName("hello", "namespace");
                foreach (DNField field in fieldSet)
                {
                    FieldType type = field.Type;
                    if (type == FieldType.SEQUENCE)
                    {
                        formatSequence(field);
                    }
                    else if (type == FieldType.GROUP)
                    {
                        Console.WriteLine("{0}Group {1}:", indent_, field.LocalName);
                        formatGroup(field);
                    }
                    else
                    {
                        Console.Write(" {0}[{1}]={2}",
                            field.LocalName,
                            field.Id,
                            field.DisplayString
                            );
                    }
                }
            }

            public void formatSequence(DNField field)
            {
                DNSequence sequence = field.Sequence;
                int size = sequence.Count;
                Console.WriteLine();
                Console.Write("{0} {1}[{2}]=", indent_, field.LocalName, field.Id);
                Console.Write("{0}Sequence: {1}[{2}] = {3}", indent_, sequence.LengthName, sequence.LengthId, size);
                String saveIndent = indent_;
                indent_ += "  ";
                int nEntry = 0;
                foreach (DNFieldSet entry in sequence)
                {
                    Console.WriteLine();
                    Console.Write("{0}[{1}]: ", indent_, nEntry++);
                    formatFieldSet(entry);
                }
                indent_ = saveIndent;
                Console.WriteLine();
            }

            public void formatGroup(DNField field)
            {
                DNFieldSet group = field.Group;
                Console.WriteLine();
                Console.Write("{0}{1}[{2}]=", indent_, field.LocalName, field.Id);
                Console.Write("{0}Group:", indent_);
                String saveIndent = indent_;
                indent_ = indent_ + "  ";
                formatFieldSet(group);
                indent_ = saveIndent;
            }
            public bool Silent
            {
                get { return silent_; }
                set { silent_ = value; }
            }
            private bool silent_ = false;
        };
    }
}
