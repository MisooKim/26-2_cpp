#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
using namespace std;

vector<string> tokenize(string text) {
    vector<string> words;
    string word = "";

    for (char c : text) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (word != "") {
                words.push_back(word);
                word = "";
            }
        }
        else {
            word += c;
        }
    }

    if (word != "")
        words.push_back(word);

    return words;
}

class Document {
public:
    string filename;
    vector<string> words;
    vector<double> tfVector;
    double similarity;

    Document(string filename) {
        this->filename = filename;
        similarity = 0.0;
    }

    void load() {
        ifstream file(filename);

        if (!file)
            throw runtime_error("파일을 열 수 없습니다: " + filename);

        string text = "";
        string line;

        while (getline(file, line))
            text += line + " ";

        words = tokenize(text);
        file.close();
    }
};

int findWord(const vector<string>& vocabulary, string word) {
    for (int i = 0; i < vocabulary.size(); i++) {
        if (vocabulary[i] == word)
            return i;
    }
    return -1;
}

vector<string> buildVocabulary(const vector<Document>& documents) {
    vector<string> vocabulary;

    for (const Document& doc : documents) {
        for (string word : doc.words) {
            if (findWord(vocabulary, word) == -1)
                vocabulary.push_back(word);
        }
    }

    return vocabulary;
}

vector<double> makeTFVector(const vector<string>& words, const vector<string>& vocabulary) {
    vector<double> tf(vocabulary.size(), 0.0);

    for (string word : words) {
        int index = findWord(vocabulary, word);
        if (index != -1)
            tf[index]++;
    }
    return tf;
}

double cosineSimilarity(const vector<double>& a, const vector<double>& b) {
    double dot = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    for (int i = 0; i < a.size(); i++) {
        dot += a[i] * b[i];
        normA += a[i] * a[i];
        normB += b[i] * b[i];
    }

    if (normA == 0 || normB == 0)
        return 0.0;

    return dot / (sqrt(normA) * sqrt(normB));
}

void printVector(const vector<double>& v) {
    cout << "[ ";

    for (double value : v)
        cout << value << " ";

    cout << "]" << endl;
}

int main() {
    try {
        vector<Document> documents;

        documents.push_back(Document("doc1.txt"));
        documents.push_back(Document("doc2.txt"));
        documents.push_back(Document("doc3.txt"));
        documents.push_back(Document("doc4.txt"));

        for (Document& doc : documents)
            doc.load();

        vector<string> vocabulary = buildVocabulary(documents);

        cout << "Vocabulary" << endl;
        cout << "[ ";

        for (string word : vocabulary)
            cout << word << " ";
        cout << "]" << endl;

        for (Document& doc : documents)
            doc.tfVector = makeTFVector(doc.words, vocabulary);

        cout << endl;
        cout << "Document TF Vectors" << endl;

        for (Document& doc : documents) {
            cout << doc.filename << " ";
            printVector(doc.tfVector);
        }

        cout << endl;
        cout << "검색어를 입력하세요: ";

        string query;
        getline(cin, query);

        vector<string> queryWords = tokenize(query);

        vector<double> queryVector = makeTFVector(queryWords, vocabulary);

        cout << "Query TF Vector ";
        printVector(queryVector);

        cout << endl;
        cout << "Search Result" << endl;

        for (Document& doc : documents) {
            doc.similarity = cosineSimilarity(queryVector, doc.tfVector);

            cout << doc.filename << " : " << doc.similarity << endl;
        }

        int bestIndex = 0;

        for (int i = 1; i < documents.size(); i++) {
            if (documents[i].similarity >
                documents[bestIndex].similarity)
                bestIndex = i;
        }

        cout << endl;
        cout << "가장 유사한 문서: " << documents[bestIndex].filename << endl;
        cout << "유사도: " << documents[bestIndex].similarity << endl;
    }
    catch (const exception& e) {
        cerr << "오류: " << e.what() << endl;
        return 1;
    }

    return 0;
}