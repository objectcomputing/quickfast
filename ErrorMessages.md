# Introduction #
When QuickFAST encounters an error it calls one of the following methods in the Context in which the error was detected.  (A Context is either a Decoder or an Encoder.)
  * reportWarning
  * reportError
  * reportFatal

> The default behavior for these methods is to throw an exception containing the error code and message.

  * Error Codes starting with 'D' are defined in the FAST protocol specification.
  * Error Codes starting with 'I' represent limitations in the QuickFAST implementation of the standard.
  * Error codes starting with 'U' are defined by QuickFAST to cover cases not mentioned in the specification.

# Definition of Error Codes #
|Error code | Typical Message | Notes |
|:----------|:----------------|:------|
|ERR D2|Unsigned or Signed Integer Field overflow.|Quite likely due to a template definition error (i.e. uInt32 when it should be uInt64)|
|ERR D4|Previous value type mismatch.|  |
|ERR D5|Missing required initial value|  |
|ERR D6|Mandatory field is missing.|  |
|ERR D7|String delta length exceeds length of previous string.|  |
|ERR D9|Unknown template name for static templateref.|  |
|ERR I1|Encoding dynamic templates is not supported.|  |
|ERR U01|Missing mandatory field.|Mismatch between Application and Template|
|ERR U02|Constant value does not match application data.|Mismatch between Application and Template|
|ERR U03|Unexpected end of data|  |
|ERR U04|Operator not supported for this data type.|An error in the template.|
|ERR U07|Segment Body not defined for Sequence instruction.|An error in the template.|
|ERR U08|Segment Body not defined for Group instruction.|An error in the template.|
|ERR U10|Constant value does not match application data.|  |
|ERR U93|Specific presence map bit not supported for this operator and data type.|Arca only|