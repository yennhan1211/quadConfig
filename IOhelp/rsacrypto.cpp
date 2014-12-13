#include "rsacrypto.h"
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <QDebug>
#include <QCryptographicHash>
#include <openssl/rsa.h>

RsaCrypto::RsaCrypto()
{
    ERR_load_CRYPTO_strings();
}

bool RsaCrypto::createRsaPrivateKey(char *key)
{
    BIO *bio = BIO_new_mem_buf((void*)key, (int)strlen(key));
    if (bio != NULL) {
        m_rsaPrivate = PEM_read_bio_RSAPrivateKey(bio, NULL, 0, NULL);
        BIO_free(bio);
        return true;
    }
    m_errorCode = ERR_get_error();
    return false;
}

bool RsaCrypto::createRsaPublicKey(char *key)
{
    BIO *bio = BIO_new_mem_buf((void*)key, (int)strlen(key));
    if (bio != NULL) {
        m_rsaPublic = PEM_read_bio_RSA_PUBKEY(bio, NULL, 0, NULL);
        BIO_free(bio);
        return true;
    }
    return false;
}

QString RsaCrypto::getErrorString(long code)
{
    char buffer[500];
    ERR_error_string(code, buffer);
    return QString::fromLocal8Bit(buffer);
}

QString RsaCrypto::getErrorString()
{
    m_errorCode = ERR_get_error();
    return getErrorString(m_errorCode);
}

unsigned long RsaCrypto::getErrorCode()
{
    return m_errorCode;
}

int RsaCrypto::encryptWithPublicKey(const QByteArray &from, QByteArray &to)
{
    if (m_rsaPublic == NULL) {
        return -1;
    }

    int rsaSize = RSA_size(m_rsaPublic);
    unsigned char sigret[rsaSize];

//    QByteArray sigmes = QCryptographicHash::hash(from, QCryptographicHash::Sha1);
    //qDebug() << sigmes.toHex();

    int siglen = RSA_public_encrypt(from.length(), (unsigned char*) (from.data()),
                                  sigret, m_rsaPublic, RSA_PKCS1_PADDING);

    if (siglen < 0) return -1;
    else {
        to.append((char*) sigret, siglen);
        return siglen;
    }
}

int RsaCrypto::decryptWithPrivateKey(const QByteArray &from, QByteArray& to)
{
    if (m_rsaPrivate == NULL) {
        return -1;
    }

    int rsaSize = RSA_size(m_rsaPrivate);
    unsigned char* buffer = new unsigned char[rsaSize];
    if(buffer == NULL)qDebug() << "buffer NULL";
    int index = 0;
    int encryptedLength = 0;

    while (index < from.size()) {
        QByteArray bytes = from.mid(index, rsaSize);
        int len = RSA_private_decrypt(bytes.length(), (unsigned char*) (bytes.data()),
                buffer, m_rsaPrivate, RSA_PKCS1_PADDING);

        if (len < 0) {
            qDebug() << "return" << getErrorString();

            return -1;
        } else {
            encryptedLength += len;
            to.append((char*) buffer,len);
            index += rsaSize;
        }
    }
    delete buffer;
    qDebug() << "len" << encryptedLength;
    return encryptedLength;
}
int RsaCrypto::sign(const QByteArray &from, QByteArray &to)
{
    if (m_rsaPrivate == 0) {
        return -1;
    }

    int rsaSize = RSA_size(m_rsaPrivate);
    unsigned int siglen;
    unsigned char sigret[rsaSize];
    QByteArray hash = QCryptographicHash::hash(from,QCryptographicHash::Sha1);
    int ret = RSA_sign(NID_sha1, (unsigned char*)hash.data(), hash.size(),
                    sigret, &siglen, m_rsaPrivate);
    to.append((char*)sigret, (int)siglen);
    qDebug() << "sig len" << siglen;
    return ret;
}

int RsaCrypto::verify(const QByteArray &text, const QByteArray &sig)
{
    if (m_rsaPublic == NULL) {
        return -1;
    }
    QByteArray hash = QCryptographicHash::hash(text,QCryptographicHash::Sha1);
    int ret = RSA_verify(NID_sha1, (unsigned char*)hash.data(), hash.size(),
               (unsigned char*)sig.data(), sig.size(), m_rsaPublic);
    qDebug() << ret;
    return ret;
}
