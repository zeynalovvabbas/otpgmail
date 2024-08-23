#include <bits/stdc++.h>
#include <curl/curl.h>
using namespace std;
string otp;
string generateOTP(int len) {
    srand((unsigned) time(NULL));
    int f = pow(10,len);
	int random = rand()%f;
	string s = to_string(random);
    return s;
}

struct upload_status {
    int bytes_read;
    string email_content;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data = upload_ctx->email_content.c_str();

    size_t buffer_size = size * nmemb;
    size_t remaining_bytes = upload_ctx->email_content.size() - upload_ctx->bytes_read;

    if (remaining_bytes == 0) {
        return 0;
    }

    size_t copy_size = (remaining_bytes < buffer_size) ? remaining_bytes : buffer_size;
    memcpy(ptr, data + upload_ctx->bytes_read, copy_size);
    upload_ctx->bytes_read += copy_size;

    return copy_size;
}
void sendMAIL(string ml){
    int len = 6;
    otp = generateOTP(len);
    string email_content =
        "To: <" + ml + ">\r\n"
        "From: <>\r\n"
        "Subject: OTP verification\r\n"
        "\r\n"
        "Your OTP is: " + otp + "\r\n";

    struct upload_status upload_ctx = { 0, email_content };

    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    string tl = "<";
    tl+=ml;
    tl+=">";
    char maill[tl.size()];
    strcpy(maill,tl.c_str());
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_USERNAME, "");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<>");
        recipients = curl_slist_append(recipients, maill);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "connection failed: " << curl_easy_strerror(res) << endl;
        }
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}



int main() {
    string mail;
    cin>>mail;
    sendMAIL(mail);
}
