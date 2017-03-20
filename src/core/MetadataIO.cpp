//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <map>

#include "../core/MetadataIO.hpp"
#include "../core/VisualTree.hpp"
#include "../core/MetadataInfo.hpp"
#include "../core/NodePhylo.hpp"


using namespace std;
using namespace pygmy;

bool MetadataIO::Read(const QString& filename, VisualTreePtr visualTree, MetadataInfoPtr metadataInfo)
{
	const int ROWS_TO_SHOW_PROGRESS_DLG = 2000;

	// clear any previously loaded metadata
	metadataInfo->Clear();

    QFile inFile (filename);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // The file could not be found
        QMessageBox msgBox;
        msgBox.setText ("Unable to open input file");
        msgBox.exec();
        return false;
    }

    QTextStream in(&inFile);

    QString first_line = in.readLine();
    if (first_line.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText ("The provided annotations file is empty");
        msgBox.exec();
        return false;
    }

    QStringList header_fields = first_line.split("\t");

    int line_number = 0;
    while (!in.atEnd()) {
        line_number++;

        QString current_line = in.readLine();

        if (current_line.isEmpty()) continue;

        QStringList fields = current_line.split("\t");
        if(fields.size() != header_fields.size()) {
            QMessageBox msgBox;
            QString msg = QString("Line %1 contains a different number of fields than expected %2 != %3").arg(line_number, fields.size(), header_fields.size());

            msgBox.setText (msg);
            msgBox.exec();
            return false;
        }
        QString &id = fields[0];
        if(id.size() == 0) {
            // no Id for the metadata
            continue;
        }
        std::map<QString, QString> current_metadata;
        for (int i = 1; i < fields.size(); i++) {
            current_metadata[header_fields[i]] = fields[i];
        }

        // associate metadata with leaf node
        NodePhylo* nodeOriginal = visualTree->GetOriginalTree()->GetNode(id);
        NodePhylo* nodeActive = visualTree->GetTree()->GetNode(id);
        if(nodeOriginal)
        {
            // failure to find a node with a given id is not necessarily an error
            // since the metadata file may simple span more sites/leaves than the
            // tree currently being considered
            nodeOriginal->SetMetadata(current_metadata);

            if(nodeActive)
                nodeActive->SetMetadata(current_metadata);

            metadataInfo->AddMetadata(current_metadata);
        }
    }
    inFile.close();
	return true;
}

