# Jira API Setup Guide

## Creating Jira API Token

### Step 1: Generate API Token
1. Go to: https://id.atlassian.com/manage-profile/security/api-tokens
2. Click **"Create API token"**
3. Give it a name: `MS Tour Agent`
4. Click **"Create"**
5. **Copy the token immediately** (you won't see it again)

### Step 2: Get Your Jira Email
- Use the email address associated with your Atlassian account
- Example: `your-email@example.com`

### Step 3: Get Your Jira Site URL
- Your site: `https://o92design.atlassian.net`

### Step 4: Store Credentials Securely

Create a file: `C:\Dev\projects\MSTour\.env` (gitignored)

```env
ATLASSIAN_EMAIL=your-email@example.com
ATLASSIAN_API_TOKEN=your-api-token-here
ATLASSIAN_BASE_URL=https://o92design.atlassian.net
JIRA_PROJECT_KEY=MST
```

**⚠️ IMPORTANT**: This file is already in `.gitignore` - never commit credentials!

## Testing the Connection

You can test your credentials with curl:

```bash
curl -u your-email@example.com:your-api-token \
  -X GET \
  -H "Content-Type: application/json" \
  https://o92design.atlassian.net/rest/api/3/myself
```

Should return your user information if credentials are valid.

## Jira REST API Endpoints

### Get Issue
```
GET /rest/api/3/issue/{issueKey}
```

### Update Issue
```
PUT /rest/api/3/issue/{issueKey}
```

### Add Comment
```
POST /rest/api/3/issue/{issueKey}/comment
```

### Update Custom Fields
```
PUT /rest/api/3/issue/{issueKey}
{
  "fields": {
    "description": "New description",
    "customfield_xxxxx": "value"
  }
}
```

## API Documentation
- Jira Cloud REST API: https://developer.atlassian.com/cloud/jira/platform/rest/v3/
- Authentication: https://developer.atlassian.com/cloud/jira/platform/basic-auth-for-rest-apis/

## Security Best Practices

1. **Never commit** `.env` file to git
2. **Use API token**, not password
3. **Rotate tokens** periodically
4. **Limit scope** if possible (read/write issues only)
5. **Delete tokens** when no longer needed

## Troubleshooting

**401 Unauthorized**
- Check email and API token are correct
- Verify token hasn't been revoked

**403 Forbidden**
- Check you have permission to edit issues
- Verify project access (MST)

**404 Not Found**
- Check issue key is correct (MST-2, MST-3)
- Verify site URL is correct

---

**Next Steps**: After creating your API token, update the `.env` file and the jira-agent can automatically update tickets!
