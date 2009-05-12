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
                    FieldType type = pair.Value.FieldType;
                    if (type == FieldType.Sequence)
                    {
                        formatSequence(pair.Key, pair.Value);
                    }
                    else if (type == FieldType.Group)
                    {
                        formatGroup(pair.Value);
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

                SequenceField sf = (SequenceField) field;
                Sequence seq = sf.Value;
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


            void formatGroup(Field field)
            {
            }


            void displayFieldValue(Field field)
            {
                switch(field.FieldType)
                {
                    case Messages.FieldType.Int32:
                        {
                            Int32Field val = (Int32Field)field;
                            out_.Write(val.Value);
                            break;
                        }
                    case Messages.FieldType.UInt32:
                        {
                            UInt32Field val = (UInt32Field)field;
                            out_.Write(val.Value);
                            break;
                        }
                    case Messages.FieldType.Int64:
                        {
                            Int64Field val = (Int64Field)field;
                            out_.Write(val.Value);
                            break;
                        }
                    case Messages.FieldType.UInt64:
                        {
                            UInt64Field val = (UInt64Field)field;
                            out_.Write(val.Value);
                            break;
                        }
                    case Messages.FieldType.Decimal:
                        {
                            DecimalField val = (DecimalField)field;
                            out_.Write(val.Value.ToDouble());
                            break;
                        }
                    case Messages.FieldType.AsciString:
                        {
                            AsciStringField val = (AsciStringField)field;
                            out_.Write(val.Value);
                            break;
                        }
                    case Messages.FieldType.UnicodeString:
                        {
                            UnicodeStringField val = (UnicodeStringField)field;
                            out_.Write(val.Value);
                            break;
                        }
                    case Messages.FieldType.ByteVector:
                        {
                            // todo: we probably should hex dump this
                            ByteVectorField val = (ByteVectorField)field;
                            out_.Write(System.Convert.ToString(val.Value));
                            break;
                        }
                    case Messages.FieldType.Sequence:
                        {
                            out_.Write("sequence");
                            break;
                        }
                    case Messages.FieldType.Group:
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
