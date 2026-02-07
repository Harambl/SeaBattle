#include "Styles.h"

// -------------------------------------------------

QString Th1StyleFactory::getStyleSheet()
{
	QString StyleSheet = R"(						
		QPushButton { background-color: #eeeeee; color: #000000; }
		QLineEdit { background-color: #ffffff; color: #000000; }
		QLabel { color: #000000; }				
	)";			 
	return StyleSheet;
}

QString Th1StyleFactory::getPlayerStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #777777; border-radius: 0px; padding: 5px;
	)";			 
	return StyleSheet;
}

QString Th1StyleFactory::getPlayerAreaStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #eeeeee;
	)";			 
	return StyleSheet;
}

QString Th1StyleFactory::getfButtonStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #dddddd; border-radius: 2px; color: #000000;
	)";			 
	return StyleSheet;
}

QString Th1StyleFactory::getPapersPath()
{
	QString PapersPath {"SeaBattlePapers1.jpg"};	
	return PapersPath;
}

// -------------------------------------------------

QString Th2StyleFactory::getStyleSheet()
{
	QString StyleSheet = R"(						
		QPushButton { background-color: #bbbbbb; color: #000000; }
		QLineEdit { background-color: #eeeeee; color: #000000; }
		QLabel { color: #000000; background-color: #eeeeee}				
	)";			 
	return StyleSheet;
}

QString Th2StyleFactory::getPlayerStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #888888; border-radius: 0px; padding: 5px;
	)";			 
	return StyleSheet;
}

QString Th2StyleFactory::getPlayerAreaStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #eeeeee;
	)";			 
	return StyleSheet;
}

QString Th2StyleFactory::getfButtonStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #dddddd; border-radius: 2px; color: #000000;
	)";			 
	return StyleSheet;
}

QString Th2StyleFactory::getPapersPath()
{
	QString PapersPath {"SeaBattlePapers2.jpg"};	
	return PapersPath;
}

// -------------------------------------------------

QString Th3StyleFactory::getStyleSheet()
{
	QString StyleSheet = R"(						
		QPushButton { background-color: #444444; color: #aaaaaa; }
		QLineEdit { background-color: #555555; color: #aaaaaa; }
		QLabel { color: #aaaaaa; }				
	)";			 
	return StyleSheet;
}

QString Th3StyleFactory::getPlayerStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #333333; color: #aaaaaa; border-radius: 0px; padding: 5px;
	)";			 
	return StyleSheet;
}

QString Th3StyleFactory::getPlayerAreaStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #777777;
	)";			 
	return StyleSheet;
}

QString Th3StyleFactory::getfButtonStyleSheet()
{
	QString StyleSheet = R"(						
		background-color: #999999; border-radius: 2px; color: #000000;
	)";			 
	return StyleSheet;
}

QString Th3StyleFactory::getPapersPath()
{
	QString PapersPath {"SeaBattlePapers3.jpg"};	
	return PapersPath;
}
