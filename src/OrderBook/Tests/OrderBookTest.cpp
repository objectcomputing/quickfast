#include <OrderBook/OrderBookPch.h>
#include <OrderBook/Book.h>
#include <Codecs/XMLTemplateParser.h>
#include <Codecs/MessageConsumer.h>
#include <Codecs/DataSourceStream.h>
#include <Codecs/SynchronousDecoder.h>
#include <Messages/Message.h>
#include <Messages/Field.h>
#include <Messages/Sequence.h>
#include <OrderBook/OrderBookMessageConsumer.h>
#include <OrderBook/OrderBookContainer.h>
#include <OrderBook/Tests/TestOrderBookListener.h>

using namespace QuickFAST;
using namespace QuickFAST::Codecs;
using namespace QuickFAST::OrderBook;

int main(int argc, char* argv[]) {
  std::ifstream templateFile;
  std::ifstream fastFile;
  std::ifstream securityIDFile;
  std::ostream* outputFile;


  templateFile.open(argv[1]);
  fastFile.open(argv[2], std::ios::in | std::ios::binary);
  securityIDFile.open(argv[3]);
  outputFile = new std::ofstream(argv[4]);

  XMLTemplateParser parser;
  TemplateRegistryPtr templateRegistry = parser.parse(templateFile);

  DataSourceStream source(fastFile);

  OrderBookContainer orderBookContainer(source, templateRegistry);

  OrderBookListenerPtr listener(new TestOrderBookListener(outputFile));

  uint32 securityIDSource;
  uint32 securityID;
  while (securityIDFile >> securityIDSource >> securityID) {
    orderBookContainer.monitorBook(SecurityID(securityIDSource, securityID), listener);
  }

  orderBookContainer.start();
  return 0;
}
