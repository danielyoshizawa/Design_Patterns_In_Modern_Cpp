#include <iostream>

// Not to create interfaces that are too long
// It's complementary to Single Responsibility Principle

struct Document
{
    std::string title{"Document"};
};

// Violating the principle
// Not all implementations will need to print, scan and fax
struct IMachine
{
    virtual void print(const Document & doc) = 0;
    virtual void scan(const Document & doc) =  0;
    virtual void fax(const Document & doc) = 0;
};

// Following the Interface Segregation Principle
// a better appoach would be to segregate the interfaces
// like the following

struct IPrinter
{
    virtual void print(const Document & doc) = 0;
};

struct IScanner
{
    virtual void scan(const Document & doc) =  0;
};

struct IFax
{
    virtual void fax(const Document & doc) = 0;
};

struct Printer : public IPrinter
{
    void print(const Document & doc) override
    {
        std::cout << "Printing..." << std::endl;
    }
};

struct Scanner : public IScanner
{
    void scan(const Document & doc) override
    {
        std::cout << "Scanning..." << std::endl;
    }
};

struct Fax : public IFax
{
    void fax (const Document & doc) override
    {
        std::cout << "Faxing..." << std::endl;
    }
};

struct IMultiFunctionPrinter : public IPrinter, public IScanner
{
};

// With the interfaces segregate I can use only the
// ones that I want
struct MultiFunctionPrinter : public IMultiFunctionPrinter
{
    IPrinter & printer;
    IScanner & scanner;

    MultiFunctionPrinter(IPrinter & printer, IScanner & scanner)
        : printer{printer},
          scanner{scanner}
    {}

    void print(const Document & doc) override
    {
        printer.print(doc);
    }

    void scan(const Document &doc) override
    {
        scanner.scan(doc);
    }
};

int main()
{
    std::cout << "Interface Segregation Principle" << std::endl;

    Printer printer;
    Scanner scanner;
    Document doc;
    MultiFunctionPrinter mfPrinter(printer, scanner);
    mfPrinter.print(doc);
    mfPrinter.scan(doc);

    return 0;
}
