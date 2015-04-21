# Release Notes #

These notes describe the highlights of QuickFAST releases.
See the ChangeLog for complete details.

## QuickFAST V1.5 ##
Released: 20130607
  * #include <Application/QuickFAST.h> is required for applications that use QuickFAST.
  * Removed support for FieldIdentityCPtr and replaced with FieldIdentity references, because ownership is not shared.
  * Added FieldSet::replaceField() to allow an existing encoded field to be replaced.
  * Works with Boost 1.53, Xerces 3.1.
  * Updated Linux setup.sh to be less intrusive.
  * Updated MPC files to build Linux Debug.
  * Updated MPC filed to use common MPC xerces base (may cause some environment changes in users)
  * Rework the MessagePerPacketAssembler to add support for A/B feeds.
  * Added CAS function for long longs.
  * Changed to maintain a long long offset in the file to be able to write files greater than 4GB.
### Issues ###
  * Addressed in this release: 81, 103, 108, 109, 110, 111, 112

## QuickFAST V1.4 ##
Released: 20111005
  * Support non-FAST message and packet headers.
  * Command line parsing moved into DecoderConfiguration to make it    consistent across applications.
  * Change multicast set-up logic to attempt to accommodate various platform quirks.
  * Numerous performance improvements.
  * Works with Boost 1.47
  * Low level support for asynchronous file I/O (on windows only).
  * Adapted to changes in MPC
  * Improved PCAP file reader.
### Issues ###
  * Addressed in this release: 69, 71, 72, 77, 80, 81, 84, 86, 91, 92, 93

## QuickFAST V1.3 ##

  * Add support for Visual Studio 2010 (AKA VC10)
  * Add a header single header file to be used by QuickFAST-based applications: src/Application/QuickFAST.h:
  * Remove .NET tests.  They need to be completely rewritten.
  * Removed !InterpretFAST and MulticastInterpreter.  The InterpretApplication provides all capabilities from these programs (and much more).
  * Allow multiple FAST connections in a single application to started and stopped independently.
  * Support both packet headers (for UDP/MULTICAST) and message headers (for both streaming and packetized data sources) independently.
  * When encoding mandatory constant values QuickFAST no longer retrieves them from the accessor.  It just assumes they are there and that they have the correct value. This cuts down on application developer's useless busywork.
  * Add MulticastSender to suppors sending FAST encoded data via multicast.
  * Change handling of groups to make merging group fields into the parent segment more under the control of the application.   This addresses a problem that showed up trying to write a round-trip test. If the group was merged by the decoder, then the generic accessor used by the encoder couldn't find it because it no longer existed as a group.  Hence the round-trip would not accurately reproduce the original input.
  * A ValueMessageBuilder that writes a FIX data file.
  * Add support for asio gather writes.
  * Change the use and management of environment variables during the build process to accomodate various development styles.
  * Change DataDestination to allow it to be reused.
  * Relax overflow check slightly for unsigned numbers.  Bits that must be sent to reach a byte boundary but which do not take part in the result will be ignored.  Previously these bits had to match the high order bit.  For signed decoding these bits must still match the sign bit.
  * Add support for nonstandard sequence encoding by Shanghai Exchange.

### ISSUES ###
  * Issues #42 through #68 and #70 through #72 are addressed by this release.
  * The following issues remain open: 4, 69, 73, and 74.

Details on issues are available under the "Issues" tab.
# Previous Releases #
Release notes for previous releases are included in the [News](News.md) page.