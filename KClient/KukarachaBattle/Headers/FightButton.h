#ifndef FIGHT_BUTTON
#define FIGHT_BUTTON

#include <QPushButton>
#include <QString>
#include <QDebug>
#include <iostream>


class FightButton : public QPushButton {
	Q_OBJECT
	QString userName;
public:
	template <typename... Ts>
	FightButton(QString userName_, Ts... args)
		: QPushButton(args...), userName{userName_} { }

	static QString fReciever;		
	const QString& getUserName() { return userName; }
	void change(bool disable)
	{
		setText(((disable == true) ? "Идет бой" : "Предложить бой"));
		setDisabled(disable);
	}


	virtual ~FightButton() { }

public slots:
	void slotSetFRecieverName()
	{
		fReciever = userName;
		emit fRecieverSet();
	}

signals:
	void fRecieverSet();
};


#endif
