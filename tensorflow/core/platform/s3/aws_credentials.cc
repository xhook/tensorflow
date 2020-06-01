/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/core/platform/s3/aws_credentials.h"

#include <aws/core/utils/memory/AWSMemory.h>
#include <aws/identity-management/auth/STSProfileCredentialsProvider.h>

#include "tensorflow/core/platform/logging.h"

namespace tensorflow {

using namespace Aws::Auth;

STSEnabledCredentialsProviderChain::STSEnabledCredentialsProviderChain()
    : DefaultAWSCredentialsProviderChain() {
  DefaultAWSCredentialsProviderChain::AddProvider(
      Aws::MakeShared<STSProfileCredentialsProvider>(
          STSEnabledCredentialsProviderChainTag));
}

// Override DefaultAWSCredentialsProviderChain::GetAWSCredentials
// to add extra logging
AWSCredentials STSEnabledCredentialsProviderChain::GetAWSCredentials() {
  for (auto&& credentialsProvider : GetProviders()) {
    AWSCredentials credentials = credentialsProvider->GetAWSCredentials();
    if (!credentials.GetAWSAccessKeyId().empty() &&
        !credentials.GetAWSSecretKey().empty()) {
      LOG(INFO) << "Using access key " << credentials.GetAWSAccessKeyId();
      return credentials;
    }
  }
  LOG(INFO) << "Could not find any credentials";
  return AWSCredentials();
}

}  // namespace tensorflow
