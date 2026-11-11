#include <iostream>
#include <string>
using namespace std;

struct Node {
    string namaPelanggan;
    Node* next;
};

struct Queue {
    Node* front;
    Node* rear;
    int count;
};

void init(Queue& q) {
    q.front = nullptr;
    q.rear = nullptr;
    q.count = 0;
}

bool isEmpty(Queue& q) {
    return q.front == nullptr;
}

void enqueue(Queue& q, string nama) {
    Node* baru = new Node;
    baru->namaPelanggan = nama;
    baru->next = nullptr;

    if (isEmpty(q)) {
        q.front = baru;
        q.rear = baru;
    } else {
        q.rear->next = baru;
        q.rear = baru;
    }
    q.count++;
}

string dequeue(Queue& q) {
    if (isEmpty(q)) {
        return "";
    }
    Node* bantu = q.front;
    string nama = bantu->namaPelanggan;
    q.front = q.front->next;

    if (q.front == nullptr) {
        q.rear = nullptr;
    }
    delete bantu;
    q.count--; 
    return nama;
}

struct NodeMenu {
    string namaMenu;
    string kategori;
    int harga;
    string status; 
    Queue waitingList;
    NodeMenu* left;
    NodeMenu* right;
};

NodeMenu* createNodeMenu(string nama, string kategori, int harga) {
    NodeMenu* baru = new NodeMenu;
    baru->namaMenu = nama;
    baru->kategori = kategori;
    baru->harga = harga;
    baru->status = "Tersedia";
    init(baru->waitingList);
    baru->left = nullptr;
    baru->right = nullptr;
    return baru;
}

struct riwayat {
    string jenis; 
    string namaMenu;
    string namaPelanggan;
};

struct NodeStack {
    riwayat data;
    NodeStack* next;
};

struct LinkedStack {
    NodeStack* top;
};

void initStack(LinkedStack& s) {
    s.top = nullptr;
}

bool isStackEmpty(LinkedStack& s) {
    return s.top == nullptr;
}

void pushStack(LinkedStack& s, riwayat aksiBaru) {
    NodeStack* baru = new NodeStack{aksiBaru, s.top};
    s.top = baru;
}

riwayat popStack(LinkedStack& s) {
    NodeStack* temp = s.top;
    riwayat dataAksi = temp->data;
    s.top = s.top->next;
    delete temp;
    return dataAksi;
}

NodeMenu* root = nullptr;
LinkedStack riwayatAksi;

void sisipBST(NodeMenu*& node, string nama, string kategori, int harga) {
    if (node == nullptr) {
        node = createNodeMenu(nama, kategori, harga);
        return;
    }
    if (nama < node->namaMenu) {
        sisipBST(node->left, nama, kategori, harga);
    } else if (nama > node->namaMenu) {
        sisipBST(node->right, nama, kategori, harga);
    }
}

NodeMenu* cariBST(NodeMenu* node, string nama) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->namaMenu == nama) {
        return node;
    }
    if (nama < node->namaMenu) {
        return cariBST(node->left, nama);
    }
    return cariBST(node->right, nama);
}

void inorderMenu(NodeMenu* node) {
    if (node == nullptr) {
        return;
    }
    inorderMenu(node->left);
    cout << "- Nama: " << node->namaMenu 
         << " | Kategori: " << node->kategori 
         << " | Harga: Rp" << node->harga 
         << " | Status: " << node->status << endl;
    inorderMenu(node->right);
}

NodeMenu* cariMin(NodeMenu* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

void hapusBST(NodeMenu*& node, string nama, bool& berhasil) {
    if (node == nullptr) return;

    if (nama < node->namaMenu) {
        hapusBST(node->left, nama, berhasil);
    } else if (nama > node->namaMenu) {
        hapusBST(node->right, nama, berhasil);
    } else {
        berhasil = true;
        if (node->left == nullptr) {
            NodeMenu* temp = node->right;
            delete node;
            node = temp;
        } else if (node->right == nullptr) {
            NodeMenu* temp = node->left;
            delete node;
            node = temp;
        } else {
            NodeMenu* temp = cariMin(node->right);
            node->namaMenu = temp->namaMenu;
            node->kategori = temp->kategori;
            node->harga = temp->harga;
            node->status = temp->status;
            node->waitingList = temp->waitingList;
            hapusBST(node->right, temp->namaMenu, berhasil);
        }
    }
}

void tampilkanAntrianHelper(NodeMenu* node, bool& adaAntrian) {
    if (node == nullptr) {
        return;
    }
    tampilkanAntrianHelper(node->left, adaAntrian);

    if (!isEmpty(node->waitingList)) {
        adaAntrian = true;
        Node* bantu = node->waitingList.front;
        while (bantu != nullptr) {
            cout << bantu->namaPelanggan << " menunggu menu " << node->namaMenu << endl;
            bantu = bantu->next;
        }
    }

    tampilkanAntrianHelper(node->right, adaAntrian);
}

void tambahMenu(string nama, string kategori, int harga) {
    sisipBST(root, nama, kategori, harga);
    cout << "Menu berhasil ditambahkan" << endl;
}

void lihatDaftarMenu() {
    if (root == nullptr) {
        cout << "Daftar menu masih kosong." << endl;
        return;
    }
    cout << "\n=== DAFTAR MENU ===" << endl;
    inorderMenu(root);
}

void catatPesanan(string namaPelanggan, string namaMenu) {
    NodeMenu* menu = cariBST(root, namaMenu);
    if (menu == nullptr) {
        cout << "Menu tidak ditemukan." << endl;
        return;
    }

    if (menu->status == "Tersedia") {
        cout << "Pesanan " << namaPelanggan << " berhasil dibuat" << endl;
        riwayat a = {"PESANAN", namaMenu, namaPelanggan};
        pushStack(riwayatAksi, a);
    } else {
        cout << "Menu sedang habis, " << namaPelanggan << " masuk ke dalam waiting list" << endl;
        enqueue(menu->waitingList, namaPelanggan);
    }
}

void menuTersediaKembali(string namaMenu) {
    NodeMenu* menu = cariBST(root, namaMenu);
    if (menu == nullptr) {
        cout << "Menu tidak ditemukan." << endl;
        return;
    }

    menu->status = "Tersedia";
    cout << "Menu tersedia kembali" << endl;

    riwayat a = {"PEMBARUAN", namaMenu, ""};
    pushStack(riwayatAksi, a);

    if (!isEmpty(menu->waitingList)) {
        string pelanggan = dequeue(menu->waitingList);
        cout << "Pesanan langsung diberikan kepada " << pelanggan << " dari waiting list" << endl;
    }
}

void hapusMenu(string namaMenu) {
    bool berhasil = false;
    hapusBST(root, namaMenu, berhasil);
    if (berhasil) {
        cout << "Menu berhasil dihapus" << endl;
    } else {
        cout << "Menu tidak ditemukan." << endl;
    }
}

void undoPesanan() {
    if (isStackEmpty(riwayatAksi)) {
        cout << "Tidak ada aksi untuk di-undo" << endl;
        return;
    }

    riwayat riwayatTerakhir = popStack(riwayatAksi);

    if (riwayatTerakhir.jenis == "PESANAN") {
        cout << "Undo: Pesanan menu " << riwayatTerakhir.namaMenu << " dibatalkan" << endl;
    } else if (riwayatTerakhir.jenis == "PEMBARUAN") {
        NodeMenu* menu = cariBST(root, riwayatTerakhir.namaMenu);
        if (menu != nullptr) {
            menu->status = "Habis";
        }
        cout << "Undo: Menu " << riwayatTerakhir.namaMenu << " kembali habis" << endl;
    }
}

void tampilkanAntrian() {
    bool adaAntrian = false;
    tampilkanAntrianHelper(root, adaAntrian);
    if (!adaAntrian) {
        cout << "Antrian kosong" << endl;
    }
}

int main() {
    initStack(riwayatAksi);
    int pilihan;

    do {
        cout << "\n==========================================" << endl;
        cout << "   SISTEM PENGELOLAAN RESTORAN KELUARGA   " << endl;
        cout << "==========================================" << endl;
        cout << "1. Tambah Menu Makanan" << endl;
        cout << "2. Lihat Daftar Menu" << endl;
        cout << "3. Catat Pesanan Pelanggan" << endl;
        cout << "4. Catat Menu Tersedia Kembali" << endl;
        cout << "5. Hapus Menu" << endl;
        cout << "6. Undo Aktivitas Terakhir" << endl;
        cout << "7. Menampilkan Antrian Pelanggan" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan Anda: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            string nama, kategori;
            int harga;
            cout << "Masukkan Nama Menu: ";
            getline(cin, nama);
            cout << "Masukkan Kategori : ";
            getline(cin, kategori);
            cout << "Masukkan Harga    : ";
            cin >> harga;
            tambahMenu(nama, kategori, harga);
        } 
        else if (pilihan == 2) {
            lihatDaftarMenu();
        } 
        else if (pilihan == 3) {
            string pelanggan, namaMenu;
            cout << "Masukkan Nama Pelanggan: ";
            getline(cin, pelanggan);
            cout << "Masukkan Nama Menu     : ";
            getline(cin, namaMenu);
            catatPesanan(pelanggan, namaMenu);
        } 
        else if (pilihan == 4) {
            string namaMenu;
            cout << "Masukkan Nama Menu: ";
            getline(cin, namaMenu);
            menuTersediaKembali(namaMenu);
        } 
        else if (pilihan == 5) {
            string namaMenu;
            cout << "Masukkan Nama Menu yang Ingin Dihapus: ";
            getline(cin, namaMenu);
            hapusMenu(namaMenu);
        } 
        else if (pilihan == 6) {
            undoPesanan();
        } 
        else if (pilihan == 7) {
            tampilkanAntrian();
        } 
        else if (pilihan == 0) {
            cout << "Program selesai." << endl;
        } 
        else {
            cout << "Pilihan tidak valid!" << endl;
        }

    } while (pilihan != 0);

    return 0;
}