#include <QWidget>
#include <water.hpp>

// stands for flourinated complicance widget
//  gets the data from the water object and displays it with a coloured chart
class ComplianceWidget : public QWidget
{
    Q_OBJECT

public:
    ComplianceWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    void createCompliance();
    void updateCompliance();
    void getComplianceData(Water water, std::string pollutant, std::string location);
}
