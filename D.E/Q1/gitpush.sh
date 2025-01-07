#!/bin/bash

# Check if the correct number of arguments are passed
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <commit-message>"
    exit 1
fi

# Predefined GitHub username and password (token)
github_username="ArjunPavanje"
github_token="123"

# Get the commit message from the argument
commit_message=$1

# Set the repository URL (you should update this to your actual repo URL)
repo_url="https://$github_username:$github_token@github.com/$github_username/your-repository.git"

# Stage all changes
git add .

# Commit with the provided message
git commit -m "$commit_message"

# Push changes to the specified branch on origin (using -u to set upstream)
git push -u origin main 

echo "Changes pushed successfully to 'main' on GitHub repository"

