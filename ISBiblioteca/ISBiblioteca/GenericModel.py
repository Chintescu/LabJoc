import os
import pandas as pd

class GenericModel:
    def __init__(self, modelName, attributesList: []):
        self.modelName = modelName
        self.set_attributes(attributesList)

    modelName = ''
    attributes = ['ID']

    def set_attributes(self, attributesList):
        databasePath = self.database_path()
        self.attributes = self.attributes + attributesList
        if os.path.exists(databasePath):
            df = pd.read_csv(databasePath)
            if set(df.columns) != set(self.attributes):
                df = df.reindex(columns=self.attributes, fill_value=None)
                df.to_csv(databasePath, index=False)
        else:
            df = pd.DataFrame(columns=self.attributes)
            df.to_csv(databasePath, index=False)

    def get_by_id(self, elementId):
        databasePath = self.database_path()
        data = pd.read_csv(databasePath)
        element = data[data['ID'] == elementId].to_dict(orient='records')
        if element:
            return element[0]
        else:
            print('Elementul nu a fost gasit')

    def get_all(self, expression=None):
        databasePath = self.database_path()
        data = pd.read_csv(databasePath)
        if expression:
            try:
                data = data.query(expression)
            except pd.errors.ParserError:
                raise ValueError(f"Expresia '{expression}' nu este valida.")
        return data.to_dict(orient='records')

    def insert(self, elements):
        if isinstance(elements, dict):
            elementsKeys = set(elements.keys()) - {'ID'}
            attributesKeys = set(self.attributes) - {'ID'}
            if elementsKeys == attributesKeys:
                databasePath = self.database_path()
                df = pd.read_csv(databasePath)
                if df.empty:
                    maxID = 0
                else:
                    maxID = df['ID'].max()
                elementsFirstKey = next(iter(elements))
                if isinstance(elements[elementsFirstKey], list):
                    numberOfElements = len(elements[elementsFirstKey])
                else:
                    numberOfElements = 1
                if 'ID' not in elements:
                    elements['ID'] = [maxID + i + 1 for i in range(numberOfElements)]
                else:
                    elements['ID'] = [maxID + 1] if not isinstance(elements['ID'], list) else [maxID + i + 1 for i in
                                                                                               range(numberOfElements)]
                df = pd.concat([df, pd.DataFrame(elements)], ignore_index=True)
                df.to_csv(databasePath, index=False)
            else:
                print('Elementele nu corespund modelului')
        elif isinstance(elements, list):
            if all(isinstance(elem, dict) for elem in elements):
                for element in elements:
                    self.insert(element)
            else:
                print('Elementele nu corespund modelului')

    def delete(self, element):
        databasePath = self.database_path()
        data = pd.read_csv(databasePath)
        data = data[data['ID'] != element['ID']]
        data.to_csv(databasePath, index=False)

    def update(self, element):
        databasePath = self.database_path()
        data = pd.read_csv(databasePath)
        if 'ID' not in element:
            raise ValueError("Elementul nu conține cheia 'ID'.")
        if not any(data['ID'] == element['ID']):
            raise ValueError(f"Nu există nicio înregistrare cu ID-ul {element['ID']} în fișierul CSV.")
        idx = data[data['ID'] == element['ID']].index[0]
        for coloana in element.keys():
            data.at[idx, coloana] = element[coloana]
        data.to_csv(databasePath, index=False)

    def database_path(self):
        scriptPath = os.path.dirname(os.path.abspath(__file__))
        databasePath = os.path.join(scriptPath, f'Databases/{self.modelName}.csv')
        return databasePath