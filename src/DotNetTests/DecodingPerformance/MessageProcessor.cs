using System;
using System.Collections.Generic;
using System.Text;
using QuickFASTDotNet.Messages;

namespace QuickFASTDotNet
{
    namespace Examples
    {
        public class MessageProcessor
        {
            private long messageTime_ = 0;

            private long int32Count_;
            private long uInt32Count_;
            private long int64Count_;
            private long uInt64Count_;
            private long decimalCount_;
            private long asciStringCount_;
            private long unicodeStringCount_;
            private long byteVectorCount_;
            private long sequenceCount_;
            private long groupCount_;
            private long nonStandardCount_;

            public MessageProcessor()
            {
                ResetCounters();
            }


            private void ResetCounters()
            {
                messageTime_ = 0;
                int32Count_ = 0;
                uInt32Count_ = 0;
                int64Count_ = 0;
                uInt64Count_ = 0;
                decimalCount_ = 0;
                asciStringCount_ = 0;
                unicodeStringCount_ = 0;
                byteVectorCount_ = 0;
                sequenceCount_ = 0;
                groupCount_ = 0;
                nonStandardCount_ = 0;
            }

            public long MesssageTime
            {
                get { return messageTime_; }
            }

            public long Int32Count
            {
                get { return int32Count_; }
            }

            public long UInt32Count
            {
                get { return uInt32Count_; }
            }

            public long Int64Count
            {
                get { return int64Count_; }
            }

            public long UInt64Count
            {
                get { return uInt64Count_; }
            }

            public long DecimalCount
            {
                get { return decimalCount_; }
            }

            public long AsciStringCount
            {
                get { return asciStringCount_; }
            }

            public long UnicodeStringCount
            {
                get { return unicodeStringCount_; }
            }

            public long ByteVectorCount
            {
                get { return byteVectorCount_; }
            }

            public long SequenceCount
            {
                get { return sequenceCount_; }
            }

            public long GroupCount
            {
                get { return groupCount_; }
            }

            public long NonStandardCount
            {
                get { return nonStandardCount_; }
            }


            public bool MessageReceived(FieldSet message)
            {
                accessMessage(message);
                return true;
            }

            void accessMessage(FieldSet message)
            {
                StopWatch parseTimer = new StopWatch();
                accessFieldSet(message);
                messageTime_ = parseTimer.freeze();
            }


            void accessFieldSet(FieldSet fieldset)
            {
                foreach (System.Collections.Generic.KeyValuePair<FieldIdentity, Field> pair in fieldset)
                {
                    FieldType type = pair.Value.FieldType;
                    if (type == FieldType.Sequence)
                    {
                        accessSequence(pair.Key, pair.Value);
                    }
                    else if (type == FieldType.Group)
                    {
                        accessGroup(pair.Value);
                    }
                    else
                    {
                        accessFieldValue(pair.Value);
                    }
                }
            }


            void accessSequence(FieldIdentity identity, Field field)
            {
                sequenceCount_++;
                SequenceField sf = (SequenceField) field;
                Sequence seq = sf.Value;
                int count = seq.Count;

                foreach (FieldSet fieldset in seq)
                {
                    accessFieldSet(fieldset);
                }
            }


            void accessGroup(Field field)
            {
                groupCount_++;
            }


            void accessFieldValue(Field field)
            {
                switch (field.FieldType)
                {
                    case Messages.FieldType.Int32:
                        {
                            int32Count_++;
                            Int32Field val = (Int32Field)field;
                            break;
                        }
                    case Messages.FieldType.UInt32:
                        {
                            uInt32Count_++;
                            UInt32Field val = (UInt32Field)field;
                            break;
                        }
                    case Messages.FieldType.Int64:
                        {
                            int64Count_++;
                            Int64Field val = (Int64Field)field;
                            break;
                        }
                    case Messages.FieldType.UInt64:
                        {
                            uInt64Count_++;
                            UInt64Field val = (UInt64Field)field;
                            break;
                        }
                    case Messages.FieldType.Decimal:
                        {
                            decimalCount_++;
                            DecimalField val = (DecimalField)field;
                            break;
                        }
                    case Messages.FieldType.AsciString:
                        {
                            asciStringCount_++;
                            AsciStringField val = (AsciStringField)field;
                            break;
                        }
                    case Messages.FieldType.UnicodeString:
                        {
                            unicodeStringCount_++;
                            UnicodeStringField val = (UnicodeStringField)field;
                            break;
                        }
                    case Messages.FieldType.ByteVector:
                        {
                            byteVectorCount_++;
                            // todo: we probably should hex dump this
                            ByteVectorField val = (ByteVectorField)field;
                            break;
                        }
                    case Messages.FieldType.Sequence:
                        {
                            // execution shouldnt be here
                            System.Console.WriteLine("ERROR: Unexpected attempt to display a sequence.");
                            sequenceCount_++;
                            break;
                        }
                    case Messages.FieldType.Group:
                        {
                            // execution shouldnt be here
                            System.Console.WriteLine("ERROR: Unexpected attempt to display a group.");
                            groupCount_++;
                            break;
                        }
                    default:
                        {
                            // execution shouldnt be here unless accessing non standard types
                            System.Console.WriteLine("WARNING: Attempted to display an unknown field type.");
                            nonStandardCount_++;
                            break;
                        }
                }
            }

        }
    }
}
