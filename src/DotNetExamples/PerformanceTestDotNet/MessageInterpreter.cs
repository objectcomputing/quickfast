using System;
using System.Collections.Generic;
using System.Text;
using QuickFASTDotNet.Messages;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        /// <summary>
        /// A copy of DotNetExamples\InterpretFASTDotNet\MessageInterpreter.cs
        /// with the console output removed.  The purpose is to force the
        /// individual message components across the native -> .NET boundary,
        /// otherwise the .NET side only has a handle to the native mesage and
        /// the performance test isn't mesauring the costs a real application
        /// would incur during decoding.
        /// </summary>
        public class MessageInterpreter
        {
            private ulong recordCount_ = 0;
            ulong recordLimit_ = 0;
            ulong recordDuplicate_ = 1;

            public MessageInterpreter(ulong recordLimit, ulong recordDuplicate)
            {
                recordCount_ = 0;
                recordLimit_ = recordLimit;
                recordDuplicate_ = recordDuplicate;
            }

            public ulong getMessageCount()
            {
                return recordCount_;
            }

            public bool MessageReceived(FieldSet message)
            {
                recordCount_ += 1;
                try
                {
                    for (ulong nDup = 0; nDup < recordDuplicate_; ++nDup)
                    {
                        formatMessage(message);
                    }

                }
                catch (Exception ex)
                {
                    Console.WriteLine("MessageReceived failed extracting data from message {0}", recordCount_);
                    Console.WriteLine(ex.ToString());
                    return false;
                }
                return recordLimit_ == 0 || recordCount_ < recordLimit_;
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
                        displayFieldValue(pair.Value);
                    }
                }
            }


            void formatSequence(FieldIdentity identity, Field field)
            {
                SequenceField sf = (SequenceField)field;
                Sequence seq = sf.Value;
                foreach (FieldSet fieldset in seq)
                {
                    formatFieldSet(fieldset);
                }
            }


            void formatGroup(Field field)
            {
                GroupField gf = (GroupField)field;
                FieldSet fs = gf.Value;
                formatFieldSet(fs);
            }


            void displayFieldValue(Field field)
            {
                switch (field.FieldType)
                {
                    case Messages.FieldType.Int32:
                        {
                            Int32Field val = (Int32Field)field;
                            break;
                        }
                    case Messages.FieldType.UInt32:
                        {
                            UInt32Field val = (UInt32Field)field;
                            break;
                        }
                    case Messages.FieldType.Int64:
                        {
                            Int64Field val = (Int64Field)field;
                            break;
                        }
                    case Messages.FieldType.UInt64:
                        {
                            UInt64Field val = (UInt64Field)field;
                            break;
                        }
                    case Messages.FieldType.Decimal:
                        {
                            DecimalField val = (DecimalField)field;
                            break;
                        }
                    case Messages.FieldType.AsciString:
                        {
                            AsciStringField val = (AsciStringField)field;
                            break;
                        }
                    case Messages.FieldType.UnicodeString:
                        {
                            UnicodeStringField val = (UnicodeStringField)field;
                            break;
                        }
                    case Messages.FieldType.ByteVector:
                        {
                            ByteVectorField val = (ByteVectorField)field;
                            break;
                        }
                    case Messages.FieldType.Sequence:
                        {
                            break;
                        }
                    case Messages.FieldType.Group:
                        {
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }

            }



        }
    }
}
