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
                int size = fieldset.Count;
                for(int nField = 0; nField < size; ++nField)
                {
                    String fieldName = "fieldset.GetNameIndexed(nField)";
                    Field field = fieldset.GetFieldIndexed(nField);

                    FieldType type = field.Type;
                    if (type == FieldType.SEQUENCE)
                    {
                        formatSequence(fieldName, field);
                    }
                    else if (type == FieldType.GROUP)
                    {
                        formatGroup(fieldName, field);
                    }
                    else
                    {
                        displayFieldValue(type,fieldName, field);
                    }
                }
            }

            void formatSequence(String name, Field field)
            {
                Sequence seq = field.toSequence;
                int count = seq.Count;
                for(int nSeq = 0; nSeq < count; ++nSeq)
                {
                    formatFieldSet(seq[nSeq]);
                }
            }


            void formatGroup(String name, Field field)
            {
                FieldSet fs = field.toGroup;
                formatFieldSet(fs);
            }


            void displayFieldValue(FieldType type, String name, Field field)
            {
                switch (type)
                {
                    case Messages.FieldType.INT32:
                        {
                            int val = field.toInt32;
                            break;
                        }
                    case Messages.FieldType.UINT32:
                        {
                            uint val = field.toUInt32;
                            break;
                        }
                    case Messages.FieldType.INT64:
                        {
                            long val = field.toInt64;
                            break;
                        }
                    case Messages.FieldType.UINT64:
                        {
                            ulong val = field.toUInt64;
                            break;
                        }
                    case Messages.FieldType.DECIMAL:
                        {
                            QuickFASTDotNet.Decimal val = field.toDecimal;
                            break;
                        }
                    case Messages.FieldType.ASCII:
                        {
                            String val = field.toAscii;
                            break;
                        }
                    case Messages.FieldType.UTF8:
                        {
                            String val = field.toUtf8;
                            break;
                        }
                    case Messages.FieldType.BYTEVECTOR:
                        {
                            byte[] val = field.toByteVector;
                            break;
                        }
                    case Messages.FieldType.SEQUENCE:
                        {
                            break;
                        }
                    case Messages.FieldType.GROUP:
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
