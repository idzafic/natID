#include <iostream>
#include <xml/Writer.h>
#include <xml/DOMParser.h>
#include <td/Date.h>
#include <td/Time.h>
#include <td/Decimal.h>
#include <thread/Thread.h>

void writeXml(const td::String& fileName)
{
    td::Date date;
    date.now();
    td::Time time;
    time.now();
    
    xml::Writer w(fileName);
    w.startDocument();
    w.startNode("Messages");
    
    w.comment("Ovo je samo demonstracija.\nPoruke se upisuju koristenjem petlje,"
              "a podaci se uzimaju iz nekog niza strukture podataka...");
    //prva poruka
    w.startNode("Message");
    int messageID = 100;
    w.attribute("id", messageID);
    w.attribute("date", date);
    w.attribute("time", time);
    w.attribute("priority", "High");
    float severity = 4.8;
    w.attribute("severity", severity);
    td::Decimal2 value = 99.95;
    w.attribute("value", value);
    w.nodeString("Ovo je prva poruka!");
    w.endNode();
    
    //druga poruka
    date.addDays(15);
    thread::sleepMilliSeconds(100); //sacekaj 100 ms
    time.now(); //update time
    w.startNode("Message");
    messageID = 101;
    w.attribute("id", messageID);
    w.attribute("date", date);
    w.attribute("time", time);
    w.attribute("priority", "Low");
    severity = 1.5;
    w.attribute("severity", severity);
    value = 200.78;
    w.attribute("value", value);
    w.nodeString("Ovo je druga poruka");
    w.endNode();
    
    //zatvori sve otvorene cvorove
    w.endDocument();
}

void readXml(const td::String& fileName)
{
    xml::FileParser parser;
    if (!parser.parseFile(fileName))
    {
        std::cout << "Ne mogu parsirati fajl: " << fileName << td::endl;
        return;
    }
    
    auto root = parser.getRootNode();
    if (root->getName().cCompare("Messages") != 0)
    {
        std::cout << "Pogresan xml fajl: " << fileName << td::endl;
        return;
    }
    
    auto message = root.getChildNode("Message");
    while (message.isOk())
    {
        int msgID = 0;
        message.getAttribValue("id", msgID);
        td::String strPriority;
        message.getAttribValue("priority", strPriority);
        float severity;
        message.getAttribValue("severity", severity);
        td::Decimal2 value;
        message.getAttribValue("value", value);
        td::Date date;
        message.getAttribValue("date", date);
        td::Time time;
        message.getAttribValue("time", time);
        td::String strMsgContent = message->getValue();
        
        std::cout << "Message id=" << msgID << ", priority=" << strPriority << ", severity=" << severity << ", value=" << value << ", date:" << date << ",time=" << time << ", txt=" << strMsgContent << td::endl;
        ++message;
    }
}

int main()
{
    td::String fileName("/Volumes/Ramdisk/Test.xml");
    writeXml(fileName);
    readXml(fileName);
    return 0; 
}
