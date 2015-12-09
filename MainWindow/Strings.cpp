#include "Strings.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <regex>
#include <QTextCodec>

Strings * Strings::_pInstance = 0;

/// <summary>
/// Konstruktor <see cref="Strings"/>.
/// Otwiera domyúlny plik ze stringami, sprawdza poprawnoúÊ rekordÛw 
/// i jeúli sπ poprawne to tworzy wpisy w mapie.
/// </summary>
Strings::Strings()
{
	QFile data("config\\strings.cfg");
	if (data.open(QFile::ReadOnly | QFile::Truncate))
	{
		QTextStream in(&data);
		in.setLocale(QLocale(QLocale::Polish, QLocale::Poland));
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
		QString str;
		std::wstring stdWStr;
		std::wsmatch matches;
		std::wregex rgx(L"([[:alnum:]_]+)\\s+=\\s+\"([[:alnum:][:punct:]_%ÍÛπú≥øüÊÒ ”•å£Øè∆—\\s]+)\"");
		while ((str = in.readLine()) != NULL)
		{
			stdWStr = str.toStdWString();
			if (std::regex_search(stdWStr, matches, rgx) && matches.size() == 3)
			{
				// zapisanie klucza i wartoúci odczytanych z pliku
				_map[QString::fromStdWString(matches.str(1))] = QString::fromStdWString(matches.str(2));
			}
		}
	}
}

void Strings::add(QString const & key, QString const & value)
{
	_map[key] = value;
}
