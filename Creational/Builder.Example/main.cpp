#include "report_builder.hpp"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>

using namespace std;

class DataParser
{
public:
    explicit DataParser(ReportBuilder& report_builder)
        : report_builder_(report_builder)
    {
    }

    virtual void Parse(const std::string& file_name)
    {
        report_builder_.add_header(string("Raport from file: ") + file_name);

        report_builder_.begin_data();

        ifstream fin(file_name.c_str());

        while (!fin.eof())
        {
            string row;
            getline(fin, row);

            DataRow data;
            string delimiters = "\t ";
            boost::split(data, row, boost::is_any_of(delimiters), boost::algorithm::token_compress_on);

            report_builder_.add_row(data);
        }

        report_builder_.end_data();

        report_builder_.add_footer("Copyright RaportBuilder 2013");
    }

    virtual ~DataParser() = default;

public:
    ReportBuilder& report_builder_;
};

HtmlDocument build_html_document()
{
    HtmlReportBuilder html_builder;

    DataParser parser(html_builder);
    parser.Parse("data.txt");

    return html_builder.get_report();
}

CsvDocument build_csv_document()
{
    CsvReportBuilder csv_builder;

    DataParser parser(csv_builder);
    parser.Parse("data.txt");

    return csv_builder.get_report();
}

int main()
{
    HtmlDocument doc_html = build_html_document();

    cout << doc_html << endl;

    ///////////////////////////////////////////////////////////
    cout << "///////////////////////////////////////////////////////////\n";

    CsvDocument csv_doc = build_csv_document();

    for (const auto& line : csv_doc)
        cout << line << endl;

    ///////////////////////////////////////////////////////////
    cout << "///////////////////////////////////////////////////////////\n";

    HtmlReportBuilder html_bldr;
    html_bldr
        .add_header("TITLE")
        .begin_data()
            .add_row({"one", "two", "three"})
            .add_row({"four", "five", "seconds"})
        .end_data()
        .add_footer("Copyright 2021");

    std::cout << html_bldr.get_report() << std::endl;
}
