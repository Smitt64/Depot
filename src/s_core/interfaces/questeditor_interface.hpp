//! class QuestEditorInterface
/*!
  Interface designed to work with the editor of questions.
  */
#ifndef QUESTEDOTOR_INTERFACE_H
#define QUESTEDOTOR_INTERFACE_H

#include <QWidget>
#include <QDomElement>

class QuestEditorInterface : public QWidget{
    Q_OBJECT
public:
    //! A constructor
    /*!
      \code
QuestEditorInterface(QWidget *parent = 0) :
    QWidget(parent)
{
}
      \endcode
      Written on the basis of class QWidget.
      \param parent as parent widget
      */
    QuestEditorInterface(QWidget *parent = 0) :
        QWidget(parent)
    {

    }
    //! A pure virtual member.
    /*!
      \sa getResource function returns a resource issue that needs to be saved in a file test
      \see getResCount
      \param id serial number of the resource.
      \param name variable that is passed the name under which the resource must be preserved.
      \param data variable that is passed to the resource conservation.
      */
    virtual void getResource(int id, QString *name, QByteArray *data) = 0;

    //! A pure virtual member.
    /*!
      \sa getResCount number of resources in question is used to save a test file
      \see getResource
      \return returns engaging of the resources.
      */
    virtual int getResCount() {return 0;}

    //! A pure virtual member.
    /*!
      \sa makeQuestionConfig generating configuration about question
      \param questElement XML branch to hold the question configuration
      */
    virtual void makeQuestionConfig(QDomElement *questElement, QDomDocument document) = 0;
    //! A pure virtual member.
    /*!
      \code
QString ClosedTestEditor::makeQuestionAlias(int questInType, int questsCount) {
    return QString("closed%1_quest%2")
            .arg(questInType)
            .arg(QTime::currentTime().msec());
}
      \endcode
      \sa makeQuestionAlias generating base question alias
      \param counter questions count
      \see makeQuestionLabel
      \return universal base question alias
      */
    virtual QString makeQuestionAlias(int counter) = 0;
    //! A pure virtual member.
    /*!
      \sa makeQuestionLabel generate text signature of question
      \see makeQuestionAlias
      \return text label that displays the question of it's essence
      */
    virtual QString makeQuestionLabel() = 0;
    //! Signals
signals:
    /*!
      \sa validationChanged triggered when the changes can save test.
      \param value is it possible to keep the issue
      */
    void validationChanged(bool value);
};

#endif //QUESTEDOTOR_INTERFACE_H
