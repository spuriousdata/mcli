from PyQt4 import QtCore as core

class Superlist(list):
    def indexOf(self, value):
        i = 0
        for x in self:
            if x == value:
                return i
            i += 1

class TreeItem(object):
    def __init__(self, data, parent=None):
        self.parentItem = parent
        self.itemData = data
        self.childItems = Superlist()
        
    def appendChild(self, item):
        self.childItems.append(item)
        
    def child(self, row):
        return self.childItems[row]
    
    def childCount(self):
        return len(self.childItems)
    
    def columnCount(self):
        return len(self.itemData)
    
    def data(self, column):
        return self.itemData[column]
    
    def parent(self):
        return self.parentItem
    
    def row(self):
        if self.parentItem is not None:
            return self.parentItem.childItems.indexOf(self)
        return 0

class TreeModel(core.QAbstractItemModel):
    def __init__(self, data, parent=None):
        super(TreeModel, self).__init__()
        rootData = Superlist()
        rootData.append("Title")
        rootData.append("Summary")
        self.rootItem = TreeItem(rootData)
        self.setupModelData(data.keys(), self.rootItem)
        
    def flags(self, item):
        if not item.isValid():
            return core.Qt.ItemIsEnabled
        return core.Qt.ItemIsEnabled | core.Qt.ItemIsSelectable
    
    def columnCount(self, parent):
        if parent.isValid():
            parent.internalPointer().columnCount()
        else:
            return self.rootItem.columnCount()
    
    def data(self, index, role):
        if not index.isValid():
            return core.QVariant()
        
        if role is not core.Qt.DisplayRole:
            return core.QVariant()

        return index.internalPointer().data(index.column())
    
    def headerData(self, section, orientation, role):
        if orientation == core.Qt.Horizontal and role == core.Qt.DisplayRole:
            return self.rootItem.data(section)
        return core.QVariant()
    
    def index(self, row, column, parent):
        if not parent.isValid():
            parentItem = self.rootItem
        else:
            parentItem = parent.internalPointer()
        
        childItem = parentItem.child(row)
        if childItem is not None:
            return self.createIndex(row, column, childItem)
        else:
            return core.QModelIndex()
        
    def parent(self, index):
        if not index.isValid():
            return core.QModelIndex()
        
        childItem = index.internalPointer()
        parentItem = childItem.parent()
        
        if parentItem == self.rootItem:
            return core.QModelIndex()
        
        return self.createIndex(parentItem.row(), 0, parentItem)
    
    def rowCount(self, parent):
        if not parent.isValid():
            parentItem = self.rootItem
        else:
            parentItem = parent.internalPointer()
        
        return parentItem.childCount()
    
    def setupModelData(self, lines, parent):
        parents = Superlist()
        indentations = Superlist()
        
        parents.append(parent)
        indentations.append(0)
        
        number = 0
        
        while number < len(lines):
            position = 0
            while position < lines[number].length():
                if lines[number].mid(position, 1) != " ":
                    break
                position += 1
            
            lineData = lines[number].mid(position).trimmed()
            
            if not lineData.isEmpty():
                columnStrings = lineData.split("\t", core.QString.SkipEmptyParts)
                columnData = Superlist()
                for column in range(0, len(columnStrings)):
                    columnData.append(columnStrings[column])
                    
                if position > indentations[-1]:
                    if parents[-1].childCount() > 0:
                        parents.append(parents[-1].child(parents[-1].childCount() - 1))
                        indentations.append(position)
                else:
                    while position < indentations[-1] and len(parents) > 0:
                        parents.pop_back()
                        indentations.pop_back()
                parents[-1].appendChild(TreeItem(columnData, parents[-1]))

            number += 1




