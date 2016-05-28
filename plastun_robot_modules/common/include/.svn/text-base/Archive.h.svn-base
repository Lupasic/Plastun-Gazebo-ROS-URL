#ifndef ARCHIVE_H_HEADER_INCLUDED_BB7523B1
#define ARCHIVE_H_HEADER_INCLUDED_BB7523B1
#include <cstring>
#include <vector>

//##ModelId=4485EB5903A9
// �����, ����������� �����.
// � ���� ����� ���������� � ��������� ������ ��������.
// ���������� �� �������, ��� ��� �� ������ ������ ��������������� (���������� ������� getData() ) � ���������� �������.
// ������ � ���� ������ FIFO, �� ���� ������, ������� ������ �������� � �����, ����������� �������.
class Archive
{
  public:

  	void clear();
    //##ModelId=4485EB710167
	// ������� ��������� ������ ������ � �����
    void push(const void* lpData, unsigned int size );

    //##ModelId=4485EB840251
	// ������� ���������� ������ ������ �� ������
    bool pop(void* lpData, unsigned int size );

	// ������� �������� ������ ������ �� ������
    void erase( unsigned int size );

    //##ModelId=44874DE503D8
	// ������� ��������� ��������� �� ��� ������ � ������
    char* getData();

	// ������� ��������� ������� ������ � ������
	int getDataSize();


  private:
    //##ModelId=44874E2302CE
	// ���������, �������������� ��� ��������
    std::vector<char> dataContainer;

};



#endif /* ARCHIVE_H_HEADER_INCLUDED_BB7523B1 */
