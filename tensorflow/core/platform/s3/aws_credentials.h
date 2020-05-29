#include <aws/core/auth/AWSCredentialsProviderChain.h>

namespace tensorflow {

static const char STSEnabledCredentialsProviderChainTag[] = "STSEnabledCredentialsProviderChain";

class STSEnabledCredentialsProviderChain : public Aws::Auth::DefaultAWSCredentialsProviderChain {
 public:
  STSEnabledCredentialsProviderChain();
  Aws::Auth::AWSCredentials GetAWSCredentials() override;
};

}  // namespace tensorflow
