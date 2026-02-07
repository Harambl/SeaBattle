#ifndef STYLES_H
#define STYLES_H

#include <QString>
#include <iostream>


class AStyleFactory {
public:
	virtual QString getStyleSheet() = 0;
	virtual QString getPlayerStyleSheet() = 0;
	virtual QString getPlayerAreaStyleSheet() = 0;
	virtual QString getfButtonStyleSheet() = 0;
	virtual QString getPapersPath() = 0;
};


class Th1StyleFactory : public AStyleFactory {
public:
	Th1StyleFactory() { }

	virtual QString getStyleSheet() override final;
	virtual QString getPlayerStyleSheet() override final;
	virtual QString getPlayerAreaStyleSheet() override final;
	virtual QString getfButtonStyleSheet() override final;
	virtual QString getPapersPath() override final;

	virtual ~Th1StyleFactory() = default;
};

class Th2StyleFactory : public AStyleFactory {
public:
	Th2StyleFactory() { }

	virtual QString getStyleSheet() override final;
	virtual QString getPlayerStyleSheet() override final;
	virtual QString getPlayerAreaStyleSheet() override final;
	virtual QString getfButtonStyleSheet() override final;
	virtual QString getPapersPath() override final;

	virtual ~Th2StyleFactory() = default;
};

class Th3StyleFactory : public AStyleFactory {
public:
	Th3StyleFactory() { }

	virtual QString getStyleSheet() override final;
	virtual QString getPlayerStyleSheet() override final;
	virtual QString getPlayerAreaStyleSheet() override final;
	virtual QString getfButtonStyleSheet() override final;
	virtual QString getPapersPath() override final;

	virtual ~Th3StyleFactory() = default;
};


#endif
