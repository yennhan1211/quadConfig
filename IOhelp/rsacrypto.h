#ifndef RSACRYPTO_H
#define RSACRYPTO_H

#include <openssl/rsa.h>
#include <QString>

class RsaCrypto
{
public:
    explicit RsaCrypto();

    bool createRsaPrivateKey(char* key);
    bool createRsaPublicKey(char* key);

    QString getErrorString(long code);
    QString getErrorString();
    unsigned long getErrorCode();

    int encryptWithPublicKey(const QByteArray& from, QByteArray& to);
    int decryptWithPrivateKey(const QByteArray& from, QByteArray& to);

private:
    unsigned long m_errorCode;
    RSA* m_rsaPrivate;
    RSA* m_rsaPublic;

};

#endif // RSACRYPTO_H
