#include <iostream>
#include <vector>
#include "tinyxml.h"

using namespace std;

class CD {
private:
    string Title;
    string Artist;
    string Country;
    string Company;
    float Price;
    int Year;

public:
    CD() = default;

    // Updated parse function to extract all fields
    int parse(TiXmlElement* CDNode) {
        if (!CDNode) return 1; // Fail if null

        // Extract and assign fields
        TiXmlElement* titleElement = CDNode->FirstChildElement("TITLE");
        TiXmlElement* artistElement = CDNode->FirstChildElement("ARTIST");
        TiXmlElement* countryElement = CDNode->FirstChildElement("COUNTRY");
        TiXmlElement* companyElement = CDNode->FirstChildElement("COMPANY");
        TiXmlElement* priceElement = CDNode->FirstChildElement("PRICE");
        TiXmlElement* yearElement = CDNode->FirstChildElement("YEAR");

        if (!(titleElement && artistElement && countryElement && companyElement &&
              priceElement && yearElement)) {
            cerr << "Not all fields of CD are present in the XML file." << endl;
            return 1;
        }

        Title = titleElement->GetText();
        Artist = artistElement->GetText();
        Country = countryElement->GetText();
        Company = companyElement->GetText();
        Price = stof(priceElement->GetText());
        Year = stoi(yearElement->GetText());

        return 0; // Success
    }

    void print() const {
        cout << "Title: " << Title << endl;
        cout << "Artist: " << Artist << endl;
        cout << "Country: " << Country << endl;
        cout << "Company: " << Company << endl;
        cout << "Price: $" << Price << endl;
        cout << "Year: " << Year << endl;
        cout << "----------------------" << endl;
    }
};

class Catalogue {
private:
    vector<CD*> cds;

public:
    Catalogue() = default;

    ~Catalogue() {
        for (size_t i = 0; i < cds.size(); i++) {
            delete cds[i];
        }
        cds.clear();
    }

    int parse(const char* filename) {
        TiXmlDocument doc;

        // Load the XML file
        if (!doc.LoadFile(filename)) {
            cerr << "Error loading file: " << doc.ErrorDesc() << endl;
            return 1;
        }

        // Get the <CATALOG> root element
        TiXmlElement* catalog = doc.FirstChildElement("CATALOG");
        if (!catalog) {
            cerr << "No <CATALOG> element found!" << endl;
            return 1;
        }

        // Iterate over all <CD> elements inside <CATALOG>
        for (TiXmlElement* eenCD = catalog->FirstChildElement("CD");
             eenCD != nullptr;
             eenCD = eenCD->NextSiblingElement("CD")) {

            auto cd = new CD; // Create a new CD object
            if (cd->parse(eenCD) == 0) {
                cds.push_back(cd); // Add successfully parsed CDs
            } else {
                delete cd; // Avoid memory leak if parsing fails
            }
        }

        return 0; // Success
    }

    void print() const {
        for (size_t i = 0; i < cds.size(); i++) {
            cds[i]->print();
        }
    }
    int getCDLength() {
        return cds.size();
    }
};

int main() {
    Catalogue catalogue;
    if (catalogue.parse("cdCatalog.xml") == 0) {
        catalogue.print();
    }
    cout << catalogue.getCDLength() << endl;
    return 0;
}
