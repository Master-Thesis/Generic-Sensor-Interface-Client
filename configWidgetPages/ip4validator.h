#ifndef IP4VALIDATOR_H
#define IP4VALIDATOR_H

#include <QValidator>

class IP4Validator : public QValidator {
public:
    IP4Validator(QObject *parent=0) : QValidator(parent){}
    void fixup(QString &input) const {}
    State validate(QString &input, int &/*pos*/) const
    {
        if(input.isEmpty()) return Acceptable;
        QStringList slist = input.split(".");
        int s = slist.size();
        if(s>4) return Invalid;
        bool emptyGroup = false;
        for(int i=0;i<s;i++)
        {
            bool ok;
            if(slist[i].trimmed().isEmpty())
            {
                emptyGroup = true;
                continue;
            }
            int val = slist[i].toInt(&ok);
            if(!ok || val<0 || val>255) return Invalid;
        }
        if(s<4 || emptyGroup) return Intermediate;
        return Acceptable;
    }
};

#endif // IP4VALIDATOR_H
