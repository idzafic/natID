//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <rpt/ReportData.h>
#include <rpt/EntityData.h>
#include <rpt/AutoElement.h>

class DocumentData : public rpt::DocumentData
{
public:
    DocumentData()
        : rpt::DocumentData()
    {
        //has to be loaded from DB
        td::DateTime now;
        now.now();
        td::DateTime in4Days(now);
        in4Days.addDays(14);

        _data[0] = "Draft Invoice"; //RPT_TXT_DOCUMENT_TYPE
        _data[1] = td::String("M1R815/15"); //RPT_TXT_DOCUMENT_ID
        _data[2] = now;	 //RPT_DT_DOCUMENT_DATETIME
        _data[3] = (td::INT4)14; //RPT_INT_DOCUMENT_PAYMENT_DAYS
        _data[4] = (td::INT4)30; //RPT_INT_DOCUMENT_EXTRA_INTEREST_RATE_AFTER_DAYS
        _data[5] = in4Days; //RPT_DT_DOCUMENT_PAYMENT_DEADLINE_DATETIME
        _data[6] = (td::INT4)0; //RPT_INT_DOCUMENT_STATE
        _data[7] = now; //RPT_DT_DOCUMENT_FROM_DATETIME
        _data[8] = now; //RPT_DT_DOCUMENT_TO_DATETIME
        _data[9] = td::String("BAM"); //RPT_TXT_DOCUMENT_MONEY_UNIT
        _data[10] = td::String("KM"); //RPT_TXT_DOCUMENT_MONEY_SYMBOL
        _data[11] = (td::INT4)7; //RPT_INT_DOCUMENT_RECLAMATION_DAYS
        _data[12] = td::String("CUST-DOC-No"); //RPT_TXT_DOCUMENT_CUST_DOC_NO;
        _data[13] = now; //RPT_DT_DOCUMENT_CUST_DOC_DT
        _data[14] = td::String("INT-DOC-No"); //RPT_DT_DOCUMENT_INT_DOC_NO;
        _data[15] = now; //RPT_DT_DOCUMENT_INT_DOC_DT
        _data[16] = td::String("Rev 1.0 (01.03.2017)"); ; //RPT_DT_DOCUMENT_REVISION
    }
};

class ObjectData : public rpt::ObjectData
{
public:
    ObjectData()
        : rpt::ObjectData()
    {
        //has to be loaded from DB
        _data[0] = "Laboratorija 1";
        _data[1] = "Glavna zgrada";
        _data[2] = "Zmaja od Bosne";
        _data[3] = "Sarajefo";
        _data[4] = "tralala la ";
        _data[5] = "033 671 612";
    }
};

class SignatureData : public rpt::SignatureData
{
public:
    SignatureData()
        : rpt::SignatureData()
    {
        //has to be loaded from DB
        _data[0] = "Niko Nikic";
        _data[1] = "Leko Lekic";
        _data[2] = "Neko Nepoznat";
    }
};

inline td::String getNote()
{
    td::String note0 = "<html><p>Crna boja a onda <span style='color:olive'>Ražnjići.</span> kao i <span style='color:crimson'> Ćevapčići.</span> za kraj.</p></html>";
//    return note0;
    td::String note1("<html><p>No<sup>2</sup><br/>te<sub>10</sub></p></html>");
//    return note1;
    
    td::String note2("<html><h1>Note 1</h1>"
        "<p><u>Normal paragraph</u> text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic no1</i></b> and<br/> ....a line break! Samo jos da dodamo m<sup>2</sup> i H<sub>2</sub>O zbog zabave. Pored ovoga ide i subscript i superscipt u bold fomatu <b>m<sup>2</sup> i H<sub>2</sub>O </b></p>"
        "<p>Another paragraph text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h2>Note 2 </h2>"
        "<p>Normal paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break 2!</p>"
        "<p>Another paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h3>Note 3 </h3>"
        "<p>Normal paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<p>Another paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break 3!</p>"
        "<h4>Note 4</h4>"
        "<p>Normal paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h5>Note 5</h5>"
        "<p>Normal paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h6>Note 6</h6>"
        "<p>Normal paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p></html>");
//    return note2;
    
    td::String note3("<html><h1>Note 1</h1>"
        "<p><u>Normal paragraph</u> text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break! Samo jos da dodamo m<sup>2</sup> i H<sub>2</sub>O zbog zabave. Pored ovoga ide i subscript i superscipt u bold fomatu <b>m<sup>2</sup> i H<sub>2</sub>O </b></p>"
        "<p>Another paragraph text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h2>Note 2 </h2>"
        "<p>Normal paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h3>Note 3 </h3>"
        "<p>Normal paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<p>Another paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h4>Note 4 </h4>"
        "<p>Normal paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h5>Note 5 </h5>"
        "<p>Normal paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h6>Note 6 </h6>"
        "<p>Normal paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h1>Note 1.1 </h1>"
        "<p><u>Normal paragraph</u> text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break! Samo jos da dodamo m<sup>2</sup> i H<sub>2</sub>O zbog zabave</p>"
        "<p>Another paragraph text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h2>Note 2.1 </h2>"
        "<p>Normal paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h3>Note 3.1 </h3>"
        "<p>Normal paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<p>Another paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h4>Note 4.1 </h4>"
        "<p>Normal paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h5>Note 5.1 </h5>"
        "<p>Normal paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h6>Note 6.1 </h6>"
        "<p>Normal paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p></html>");
    
    td::String note4("<html><h1>Note 1</h1>"
        "<p><u>Normal paragraph</u> text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and .... no line break! Samo jos da dodamo m<sup>2</sup> i H<sub>2</sub>O zbog zabave. Pored ovoga ide i subscript i superscipt u <span style=\"color:red\">bold</span> fomatu <b>m<sup>2</sup> i H<sub>2</sub>O </b>. Evo i par bojica <span style=\"color:crimson\">Ćevapčići.</span>  <span style = \"color:olive\">Ražnjići. </span> <span style=\"color:violet\">Ćevapčići. </span> - <span style = \"color:olive\">Ražnjići. </span> <span style=\"color:crimson\">Ćevapčići. </span> - <span style = \"color:khaki\">Ražnjići. </span> <span style=\"color:pink\">Ćevapčići. </span> - <span style = \"color:darkGray\">Ražnjići. </span> <span style=\"color:darkBlue\">Ćevapčići. </span> - <span style = \"color:maroon\">Ražnjići. </span> <b> A sad <span style=\"color:darkRed\">Ćevapčići. </span> - <span style = \"color:olive\">Ražnjići i još nešto na drugoj strani bi trebalo da se pojavi u bold formatu i olive boji. </span> </b> <span style=\"color:crimson\">Ćevapčići. </span> - <span style = \"color:olive\">Ražnjići. </span> <span style=\"color:crimson\">Ćevapčići. </span> - <span style = \"color:olive\">Ražnjići. </span></p>"
        "<p>Another paragraph text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h2>Note 2 </h2>"
        "<p>Normal paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h3>Note 3</h3>"
        "<p>Normal paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<p>Another paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h4>Note 4 </h4>"
        "<p>Normal paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h5>Note 5</h5>"
        "<p>Normal paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h6>Note 6 </h6>"
        "<p>Normal paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h1>Note 1.1</h1>"
        "<p><u>Normal paragraph</u> text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break! Samo jos da dodamo m<sup>2</sup> i H<sub>2</sub>O zbog zabave</p>"
        "<p>Another paragraph text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h2>Note 2.1 </h2>"
        "<p>Normal paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h3>Note 3.1</h3>"
        "<p>Normal paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<p>Another paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h4>Note 4.1 </h4>"
        "<p>Normal paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h5>Note 5.1</h5>"
        "<p>Normal paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h6>Note 6.1 </h6>"
        "<p>Normal paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h1>Note 1.2</h1>"
        "<p><u>Normal paragraph</u> text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break! Samo jos da dodamo m<sup>2</sup> i H<sub>2</sub>O zbog zabave</p>"
        "<p>Another paragraph text in area of H1. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h2>Note 2.2 </h2>"
        "<p>Normal paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H2. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<h3>Note 3.2</h3>"
        "<p>Normal paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and"
        "....no line break unless it cannot fit in the specified rectangle!</p>"
        "<p>Another paragraph text in area of H3. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h4>Note 4.2 </h4>"
        "<p>Normal paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H4. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and <br/>....a line break!</p>"
        "<h5>Note 5.2</h5>"
        "<p>Normal paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H5. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<h6>Note 6.2 </h6>"
        "<p>Normal paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p>"
        "<p>Another paragraph text in area of H6. <b>Some bold</b> and of course <i>some italic.</i> We would also like to see <b><i>bold - italic</i></b> and<br/>....a line break!</p></html>");
    return note4;
}

class CompanyData : public rpt::CompanyData
{
public:
    CompanyData()
        : rpt::CompanyData()
    {
        //has to be loaded from DB
        _data[0] = "UNIS \"TOK\" d.o.o.";
        _data[1] = "Tvornica za proizvodnju odbojno vlacnih naprava i otkivaka";
        _data[2] = "Zmaja od Bosne bb, 71000 Sarajevo";
        _data[3] = "---ddd--";
        //etc
    }
};

