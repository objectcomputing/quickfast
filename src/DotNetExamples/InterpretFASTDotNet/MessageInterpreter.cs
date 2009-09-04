using System;
using System.Collections.Generic;
using System.Text;
using QuickFASTDotNet.Messages;



namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class MessageInterpreter
        {
            private ulong recordCount_ = 0;
            private ulong indent_ = 0;
            System.IO.TextWriter out_;

            public MessageInterpreter(System.IO.TextWriter outputStream)
            {
                recordCount_ = 0;
                indent_ = 0;
                out_ = outputStream;
            }


            public bool MessageReceived(FieldSet message)
            {
                recordCount_ += 1;
                out_.Write("Record #{0} ",  recordCount_);
                out_.Flush();
                try
                {
                    formatMessage(message);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("MessageReceived failed extracting data from message {0}", recordCount_);
                    Console.WriteLine(ex.ToString());
                    return false;
                }
                out_.WriteLine();
                return true;
            }


            void formatMessage(FieldSet message)
            {

                formatFieldSet(message);
            }


            void formatFieldSet(FieldSet fieldset)
            {

                foreach (System.Collections.Generic.KeyValuePair<FieldIdentity, Field> pair in fieldset)
                {
                    FieldType type = pair.Value.Type;
                    if (type == FieldType.SEQUENCE)
                    {
                        formatSequence(pair.Key, pair.Value);
                    }
                    else if (type == FieldType.GROUP)
                    {
                        formatGroup(pair.Key, pair.Value);
                    }
                    else
                    {
                        out_.Write(" {0}[{1}]=", pair.Key.Name, pair.Key.Id);
                        displayFieldValue(pair.Value);
                    }
                }
            }


            void newline()
            {
                out_.WriteLine();
                for (ulong cnt = 0; cnt < indent_ * 2; cnt++)
                {
                    out_.Write(' ');
                }
            }


            void formatSequence(FieldIdentity identity, Field field)
            {

                Sequence seq = field.toSequence;
                int count = seq.Count;

                newline();
                out_.Write(" {0}[{1}]=", identity.Name, identity.Id);
                out_.Write("Sequence[{0}]", count);
                ulong entryCount = 0;
                ++indent_;
                foreach (FieldSet fieldset in seq)
                {
                    newline();
                    out_.Write("[{0}]: ", entryCount++);
                    formatFieldSet(fieldset);
                }
                --indent_;
                newline();
            }


            void formatGroup(FieldIdentity identity, Field field)
            {
                FieldSet group = field.toGroup;
                newline();
                out_.Write("{0}[{1}] = Group", identity.Name, identity.Id);
                ++indent_;
                formatFieldSet(group);
                --indent_;
            }


            void displayFieldValue(Field field)
            {
                switch(field.Type)
                {
                    case Messages.FieldType.INT32:
                        {
                            out_.Write(field.toInt32);
                            break;
                        }
                    case Messages.FieldType.UINT32:
                        {
                            out_.Write(field.toUInt32);
                            break;
                        }
                    case Messages.FieldType.INT64:
                        {
                            out_.Write(field.toInt64);
                            break;
                        }
                    case Messages.FieldType.UINT64:
                        {
                            out_.Write(field.toUInt64);
                            break;
                        }
                    case Messages.FieldType.DECIMAL:
                        {
                            out_.Write(field.toDecimal.ToDouble());
                            break;
                        }
                    case Messages.FieldType.ASCII:
                        {
                            out_.Write(field.toAscii);
                            break;
                        }
                    case Messages.FieldType.UTF8:
                        {
                            out_.Write(field.toUtf8);
                            break;
                        }
                    case Messages.FieldType.BYTEVECTOR:
                        {
                            // todo: we probably should hex dump this
                            out_.Write(System.Convert.ToString(field.toByteVector));
                            break;
                        }
                    case Messages.FieldType.SEQUENCE:
                        {
                            out_.Write("sequence");
                            break;
                        }
                    case Messages.FieldType.GROUP:
                        {
                            out_.Write("group");
                            break;
                        }
                    default:
                        {
                            out_.Write("unknown field type");
                            break;
                        }
                }

            }



        }
    }
}
